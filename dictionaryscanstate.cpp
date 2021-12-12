#include "dictionaryscanstate.h" 

namespace DLP
{

DictionaryScanState::DictionaryScanState(HSRegexEngine* regexEngine, LitRegexEngine* litRegexEngine) {
    regexScanState_ = regexEngine->CreateRegexScanState();
    litScanState_ = litRegexEngine->CreateRegexScanState();
}

DictionaryScanState::~DictionaryScanState() {
}

}
 
