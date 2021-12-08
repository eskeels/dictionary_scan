#include <iostream>

#include <hs.h>

#include "hsbaseregexengine.h"
#include "dictionarytermid.h"

namespace DLP {

    void HSBaseRegexEngine::RegisterDictionaryItem(uint16_t dictionaryId, const DictionaryItem* di) {
std::cout << "term is " << di->GetRegex() << std::endl;
        expressions_.push_back(di->GetRegex().c_str());
        uint16_t termId = di->GetId();
        // each id holds the dictionary id and term id
        DictionaryTermId dti(dictionaryId, termId);
        ids_.push_back(dti.GetId());
        flags_.push_back(GetFlags(di)); // |HS_FLAG_SOM_LEFTMOST);
        lens_.push_back(di->GetRegex().size());
    }
}

