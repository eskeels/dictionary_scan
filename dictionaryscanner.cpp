#include <iostream>

#include "dictionaryscanner.h"
#include "dictionaryscanstate.h"

namespace DLP {
DictionaryScanner::DictionaryScanner(const Dictionaries* ds)
   : dictionaries_(ds) {
    regexEngine_ = new HSRegexEngine();
    litRegexEngine_ = new LitRegexEngine();
}

DictionaryScanner::~DictionaryScanner() {
}

void DictionaryScanner::Initialize(const std::vector<uint16_t>& dictionaryIds) {
    regexEngine_->Register(dictionaries_);
    litRegexEngine_->Register(dictionaries_);
    regexEngine_->Initialize();
    litRegexEngine_->Initialize();
}

IScanState* DictionaryScanner::CreateScanState() const {
    return new DictionaryScanState(regexEngine_,litRegexEngine_);
}

void DictionaryScanner::Scan(IScanState* ss, size_t offset, const char * input, const char * normalized) const {
   // scan with regex engines 
    DictionaryScanState* dss = static_cast<DictionaryScanState*>(ss);

    regexScanState_
    litScanState_
}

}
