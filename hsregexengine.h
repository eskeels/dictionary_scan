#pragma once

#include <hs.h>

#include "hsbaseregexengine.h"
#include "dictionaries.h"

namespace DLP {
class HSRegexEngine : public HSBaseRegexEngine {
    public:
        void Initialize();
        void Serialize();
        unsigned GetFlags(const DictionaryItem* /*di*/) {
            return HS_FLAG_MULTILINE|HS_FLAG_DOTALL|HS_FLAG_UTF8|HS_FLAG_UCP;
        }
};
}
