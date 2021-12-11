#pragma once

#include <stdint.h>

#include <vector>

#include "iscanner.h"
#include "iscanstate.h"

#include "dictionaries.h"
#include "hsregexengine.h"
#include "litregexengine.h"

namespace DLP
{
class DictionaryScanState;

class DictionaryScanner : public IScanner {
    public:
        DictionaryScanner(const Dictionaries* ds);
        ~DictionaryScanner();
    
        void Initialize(const std::vector<uint16_t>& dictionaryIds);

        IScanState* CreateScanState() const;

        void Scan(IScanState* ss, size_t offset, const char * input, const char * normalized) const;

    protected:
        const Dictionaries* dictionaries_;
        HSRegexEngine* regexEngine_ = nullptr;
        LitRegexEngine* litRegexEngine_ = nullptr;
};
}
