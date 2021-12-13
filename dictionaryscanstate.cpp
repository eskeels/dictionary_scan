#include "dictionaryscanstate.h" 

namespace DLP
{

DictionaryScanState::DictionaryScanState(HSRegexEngine* regexEngine, LitRegexEngine* litRegexEngine) {
    if (regexEngine->GetItemCount() > 0) {
        regexScanState_ = regexEngine->CreateRegexScanState();
    }

    if (litRegexEngine->GetItemCount() > 0) {
        litScanState_ = litRegexEngine->CreateRegexScanState();
    }
}

DictionaryScanState::~DictionaryScanState() {
}

}
 
