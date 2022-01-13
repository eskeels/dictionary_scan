#pragma once

#include <stdint.h>

#include <vector>

#include "iscanner.h"
#include "iscanstate.h"

#include "dictionaries.h"
#include "hsregexengine.h"
#include "litregexengine.h"
#include "dictionaryscanstate.h"
#include "dictionaryscanmatches.h"

namespace DLP
{
class DictionaryScanner : public IScanner {
    public:
        DictionaryScanner(const Dictionaries* ds);
        ~DictionaryScanner();
    
        void Initialize(const std::vector<uint16_t>& dictionaryIds);

        IScanState* CreateScanState() const;

        void Scan(IScanMatches* sm, IScanState* ss, size_t offset, size_t overlap, const char * input, size_t ilen, const char * normalized, size_t nlen, uint8_t context, size_t matchLimit = 100000) const;

    protected:
        const Dictionaries* dictionaries_;
        HSRegexEngine* regexEngine_ = nullptr;
        LitRegexEngine* litRegexEngine_ = nullptr;
};
}
