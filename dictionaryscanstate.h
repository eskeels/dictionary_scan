#pragma once

#include <stdint.h>

#include "iscanstate.h"
#include "iregexscanstate.h"
#include "hsregexengine.h"
#include "litregexengine.h"

namespace DLP
{
class DictionaryScanState : public IScanState {
    public:
        DictionaryScanState(HSRegexEngine* regexEngine, LitRegexEngine* litRegexEngine);
        ~DictionaryScanState();
        IRegexScanState* GetRegexScanState() { return regexEngine_; }
        IRegexScanState* GetLitScanState() { return litRegexEngine_; }
    protected:
        IRegexScanState* regexScanState_ = nullptr;
        IRegexScanState* litScanState_ = nullptr;
};
}
 
