#pragma once

#include "hsbaseregexengine.h"
#include "dictionaries.h"

namespace DLP {
class LitRegexEngine : public HSBaseRegexEngine {
    public:
        virtual ~LitRegexEngine();
        void RegisterDictionaryItem(uint16_t dictionaryId, const IDictionaryItem* di);

        void Initialize();
        void Serialize(); 
        unsigned GetFlags(const IDictionaryItem* di) {
            unsigned flag = HS_FLAG_CASELESS|HS_FLAG_SOM_LEFTMOST;
            if (di->IsCaseSensitive()) {
                flag |= HS_FLAG_CASELESS;
            }
  //          if (di->IsDistinct()) {
    //            flag |= HS_FLAG_SINGLEMATCH;
      //      }
            return flag;
        }
};
}
