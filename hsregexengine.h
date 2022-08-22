#pragma once

#include<iostream>
#include<fstream>

#include <hs.h>

#include "hsbaseregexengine.h"
#include "dictionaries.h"

namespace DLP {
class HSRegexEngine : public HSBaseRegexEngine {
    public:
        ~HSRegexEngine();
        void RegisterDictionaryItem(uint16_t dictionaryId, const IDictionaryItem* di);
        using HSBaseRegexEngine::Initialize;
        bool Initialize();
        bool Serialize(char **bytes, size_t *length, std::string& errDesc);

        unsigned GetFlags(const IDictionaryItem* di) {
            unsigned flag = HS_FLAG_MULTILINE|HS_FLAG_DOTALL|HS_FLAG_UTF8; //|HS_FLAG_UCP;
            if (di->IsCaseSensitive()) {
                flag |= HS_FLAG_CASELESS;
            }
//            if (di->IsDistinct()) {
  //              flag |= HS_FLAG_SINGLEMATCH;
    //        }
            return flag;
        }
};
}
