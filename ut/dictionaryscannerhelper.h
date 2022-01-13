#pragma once

#include <memory>
#include <regex>

#include "gtest/gtest.h"

#include "hsregexengine.h"
#include "litregexengine.h"
#include "dictionaryitemfactory.h"
#include "dictionaryscanner.h"
#include "dictionaryscanmatches.h"

using namespace DLP;
enum TermType { LITERAL, REGEX };

void AddDictionary( DLP::Dictionaries& ds,
                    const std::string& name,
                    uint16_t id,
                    const std::vector<std::string> terms,
                    TermType ttype,
                    int16_t score = 20,
                    bool partial = false,
                    bool distiinct = false,
                    bool caseSens = true );

void AddDictionary( DLP::Dictionaries& ds,
                    const std::string& name,
                    uint16_t id,
                    const std::vector<std::string> terms,
                    TermType ttype,
                    int16_t score,
                    bool partial,
                    bool distiinct,
                    bool caseSens) {

    DLP::DictionaryItemFactory ifactory;
    ifactory.SetDefaults(score,
            distiinct,      // distinct
            partial,        // partial
            caseSens,       // case sensitive
            10);            // distance

    DLP::Dictionary* d = new Dictionary(name, id, 1);
    for ( auto& t : terms ) {
        if (ttype == LITERAL) {
            d->Add(ifactory.CreateLiteral(t,nullptr,nullptr,nullptr,nullptr));
        } else {
            d->Add(ifactory.CreateRegex(t,nullptr,nullptr,nullptr,nullptr));
        }
    }
    ds.Add(d);
}

std::string getWord();

std::string getWord() {
    std::string tmp; // ("\\W");    
    size_t len = 3+(rand() % 9);
    for (size_t i = 0; i < len; ++i) {
        int randomChar = rand()%(26+26+10);
        if (randomChar < 26)
            tmp.append(1, 'a' + randomChar);
        else if (randomChar < 26+26)
            tmp.append(1, 'A' + randomChar - 26);
        else
            tmp.append(1,'0' + randomChar - 26 - 26);
    }
    return tmp;//+"\\W";
}

class VMatch {
    public:
        VMatch(size_t from, size_t to) : from_(from), to_(to) {}
        VMatch(size_t from, size_t to, uint16_t did) 
            : from_(from),
              to_(to),
              dictionaryId_(did) {}

    bool HasDictionaryId() const {
        return (dictionaryId_ != 0);
    }

    uint16_t GetDictionaryId() const {
        return dictionaryId_;
    }

    size_t from_;
    size_t to_;
    uint16_t dictionaryId_ = 0;
};

DLP::DictionaryScanMatches Scan(DLP::Dictionaries& ds, const std::string& txt, uint8_t context);
DLP::DictionaryScanMatches Scan(DLP::Dictionaries& ds, const std::string& txt, uint8_t context) {
    DLP::DictionaryScanner dscanner(&ds);
    dscanner.Initialize({});
    std::unique_ptr<IScanState> ss(dscanner.CreateScanState());
    DLP::DictionaryScanMatches dsm(&ds);
    dscanner.Scan(&dsm, &*ss, 0, 0, txt.c_str(), txt.size(), txt.c_str(), txt.size(), context);
    return dsm;
}

bool ScanAndVerify3(DLP::Dictionaries& ds, const std::string& txt, std::vector<VMatch> matches, int64_t& totalScore, DLP::DictionaryScanMatches* pDSM = nullptr, uint8_t context = 1);
bool ScanAndVerify3(DLP::Dictionaries& ds, const std::string& txt, std::vector<VMatch> matches, int64_t& totalScore, DLP::DictionaryScanMatches* pDSM, uint8_t context) {
    DLP::DictionaryScanMatches dsm = Scan(ds, txt, context);

    if (dsm.GetMatchCount() != matches.size()){
        std::cout << "Match count mismatch! Expected " << matches.size() << " but got " << dsm.GetMatchCount() << std::endl;
        return false;
    }

    totalScore = dsm.GetTotalScore();

    for (auto& vm : matches) {
        size_t idx;
        size_t matchIdx = 0;
        const Match* m = dsm.GetFirstMatch(idx, vm.GetDictionaryId());
        bool found = false;
        if (m) {
            while(m) {
                std::cout << idx << std::endl;
                if (m->GetFrom() == vm.from_
                    &&
                    (m->HasFrom()==false || m->GetTo() == vm.to_)) {
                    std::cout << "found!" << std::endl;
                    found = true;
                } else {
                    m = dsm.GetNextMatch(idx);
                    matchIdx++;
                }
                if (found) {
                    break;
                }
            }
            if (!found) {
                std::cout << "Failed to find match with start pos " << m->GetFrom() << std::endl;
                return false;
            }
        } else {
            std::cout << "Failed to find match in dictionary start pos " << m->GetFrom() << std::endl;
            return false; 
        }
    }

    size_t idx;
    size_t matchIdx = 0;
    const Match* m = dsm.GetFirstMatch(idx);
    while(m) {
        std::cout << idx << std::endl;
        if (m->HasFrom() && (m->GetFrom() != matches[matchIdx].from_)) {
            std::cout << "Failed to match from : " << m->GetFrom() << " != " << matches[matchIdx].from_ << std::endl;
            return false;
        }
        if (m->GetTo() != matches[matchIdx].to_) {
            std::cout << "Failed to match to : " << m->GetTo() << " != " << matches[matchIdx].to_ << std::endl;
            return false;
        }
        m = dsm.GetNextMatch(idx);
        matchIdx++;
    }

    if (pDSM) {
        *pDSM = dsm;
    }

    return true;
}

bool ScanAndVerify2(DLP::Dictionaries& ds, const std::string& txt, std::vector<VMatch> matches, DLP::DictionaryScanMatches* pDSM = nullptr, uint8_t context = 1);
bool ScanAndVerify2(DLP::Dictionaries& ds, const std::string& txt, std::vector<VMatch> matches, DLP::DictionaryScanMatches* pDSM, uint8_t context) { 
    int64_t totalScore = 0;
    return ScanAndVerify3(ds, txt, matches, totalScore, pDSM, context);
}

