#include <hs.h>

#include <iostream>

#include "hsregexengine.h"
#include "dictionarytermid.h"

namespace DLP {

    void HSRegexEngine::RegisterDictionaryItem(uint16_t dictionaryId, const DictionaryItem* di) {
std::cout << "Regex " << di->GetRegex() << std::endl;
        expressions_.push_back(di->GetRegex().c_str());
        uint16_t termId = di->GetId();
        // each id holds the dictionary id and term id
        DictionaryTermId dti(dictionaryId, termId);
        ids_.push_back(dti.GetId());
        flags_.push_back(HS_FLAG_MULTILINE|HS_FLAG_DOTALL|HS_FLAG_UTF8|HS_FLAG_UCP|HS_FLAG_SOM_LEFTMOST);

    }

    void HSRegexEngine::Initialize() {
        // TODO: Init HS
    }

}

