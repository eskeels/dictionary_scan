#include <iostream>

#include "dictionaryscanner.h"
#include "dictionaryscanstate.h"
#include "hsregexscanstate.h"
#include "dictionaryscanmatches.h"
#include "dictionarytermid.h"

namespace DLP {
DictionaryScanner::DictionaryScanner(const Dictionaries* ds)
   : dictionaries_(ds) {
    regexEngine_ = new HSRegexEngine();
    litRegexEngine_ = new LitRegexEngine();
}

DictionaryScanner::~DictionaryScanner() {
}

std::vector<char> readFile(const std::string& filename)
{
    // open the file:
    std::basic_ifstream<char> file(filename.c_str(), std::ios::binary);

    // read the data:
    return std::vector<char>((std::istreambuf_iterator<char>(file)),
                              std::istreambuf_iterator<char>());
}

bool DictionaryScanner::Initialize(const std::vector<uint16_t>& /*dictionaryIds*/, 
                                    const std::string& literalDBFileName,
                                    const std::string& regexDBFileName,
                                    std::string& errorDesc) {
    bool ret = false;
    regexEngine_->Register(dictionaries_);
    litRegexEngine_->Register(dictionaries_);
    if (regexEngine_->GetItemCount() > 0) {
        if (!regexDBFileName.empty()) {
            std::vector<char> dbFromFile = readFile(regexDBFileName);
            ret = regexEngine_->Initialize((char *)&dbFromFile[0], (size_t)dbFromFile.size(), errorDesc);
        } else {
            ret = regexEngine_->Initialize();
        }
    }

    if (litRegexEngine_->GetItemCount() > 0) {
        if (!literalDBFileName.empty()) {
            std::vector<char> dbFromFile = readFile(literalDBFileName);
            ret = litRegexEngine_->Initialize((char *)&dbFromFile[0], (size_t)dbFromFile.size(), errorDesc);    
        } else {
            ret = litRegexEngine_->Initialize();
        }
    }

    return ret;
}

IScanState* DictionaryScanner::CreateScanState() const {
    return new DictionaryScanState(regexEngine_,litRegexEngine_);
}

/**
 * This is the function that will be called for each match that occurs. @a ctx
 * is to allow you to have some application-specific state that you will get
 * access to for each match. In our simple example we're just going to use it
 * to pass in the pattern that was being searched for so we can print it out.
 */
static int regexEvent(unsigned int id, unsigned long long /*from*/,
                        unsigned long long to, unsigned int /*flags*/, void *ctx) {

    DictionaryTermId dti(id);

    DictionaryScanMatches* dss = static_cast<DictionaryScanMatches*>(ctx);
    dss->RecordMatch(dti.GetDictionaryId(), dti.GetTermId(), to);
    return 0;
}

static int literalEvent(unsigned int id, unsigned long long from,
                        unsigned long long to, unsigned int /*flags*/, void *ctx) {

    DictionaryTermId dti(id);

    DictionaryScanMatches* dss = static_cast<DictionaryScanMatches*>(ctx);
    dss->RecordMatch(dti.GetDictionaryId(), dti.GetTermId(), from, to);
    return 0;
}

void DictionaryScanner::Scan(IScanMatches* sm, IScanState* ss, size_t offset, size_t overlap, const char * input, size_t ilen, const char * normalized, size_t nlen, uint8_t context, size_t matchLimit) const {
    // scan with regex engines 
    DictionaryScanMatches* dsm = static_cast<DictionaryScanMatches*>(sm);
    dsm->SetInputBuffer(input, ilen);
    dsm->SetContext(context);
    dsm->SetOffset(offset);
    dsm->SetOverlap(overlap);
    dsm->SetMatchLimit(matchLimit);
    DictionaryScanState* dss = static_cast<DictionaryScanState*>(ss);
    HSRegexScanState* rss = static_cast<HSRegexScanState*>(dss->GetRegexScanState());
    HSRegexScanState* lss = static_cast<HSRegexScanState*>(dss->GetLitScanState());
    if (regexEngine_->GetItemCount() > 0) {
        if (hs_scan(regexEngine_->GetDatabase(), input, ilen, 0, rss->GetScratch(), regexEvent,
                    dsm) != HS_SUCCESS) {
            printf("REGEX ERROR FROM SCAN!!!");
        }
    }

    if (litRegexEngine_->GetItemCount() > 0) {
        if (hs_scan(litRegexEngine_->GetDatabase(), normalized, nlen, 0, lss->GetScratch(), literalEvent,
                    dsm) != HS_SUCCESS) {
            printf("LIT ERROR FROM SCAN!!!");
        }
    }
    dsm->ResolveProximity();
}

bool writeDBToFile(const std::string& filename, IRegexEngine* regexEngine, std::string& errorDesc) {
    bool ret = true;
    if (regexEngine->GetItemCount()) {
        char *bytesDb = nullptr;
        size_t length;
        std::ofstream outFile(filename.c_str(), std::ios::out | std::ios::binary);
        std::cout << "Count is " << regexEngine->GetItemCount() << std::endl;
        ret = regexEngine->Serialize(&bytesDb, &length, errorDesc);
        if (ret) {
            outFile.write(bytesDb, length);
            outFile.close();
            free(bytesDb);
        }
    }
    return ret;
}
 
bool DictionaryScanner::Serialize(const std::string& literalDBFileName, const std::string& regexDBFileName, std::string& errorDesc) const {
    bool ret = writeDBToFile(literalDBFileName, litRegexEngine_, errorDesc);
    if (ret) {
        writeDBToFile(regexDBFileName, regexEngine_, errorDesc);
    }

    return ret;
}

}
