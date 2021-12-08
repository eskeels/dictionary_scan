#pragma once

#include <chimera/ch.h>

#include "hsbaseregexengine.h"
#include "dictionaries.h"

namespace DLP {
class LitRegexEngine : public HSBaseRegexEngine {
    public:
        void Initialize();
        void Serialize(); 
        unsigned GetFlags(const DictionaryItem* /*di*/) {
            return HS_FLAG_CASELESS|HS_FLAG_SOM_LEFTMOST;
        }
};
}
