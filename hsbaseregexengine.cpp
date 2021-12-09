#include <iostream>

#include <hs.h>

#include "hsbaseregexengine.h"
#include "dictionarytermid.h"

namespace DLP {
    HSBaseRegexEngine::~HSBaseRegexEngine() {
        hs_free_database(database_);
    }

    void HSBaseRegexEngine::RegisterDictionaryItem(uint16_t dictionaryId, const IDictionaryItem* di) {
// std::cout << "term is " << di->GetRegex() << std::endl;
        expressions_.push_back(di->GetRegex().c_str());
        uint16_t termId = di->GetId();
        // each id holds the dictionary id and term id
        DictionaryTermId dti(dictionaryId, termId);
        ids_.push_back(dti.GetId());
        flags_.push_back(GetFlags(di));
        lens_.push_back(di->GetRegex().size());
    }
}

