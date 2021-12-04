#include "regexengine.h"
#include "dictionarytermid.h"

namespace DLP {

void RegexEngine::Initialise(const Dictionaries* ds) {
    size_t didx = 0;
    const Dictionary* d = ds->GetFirstDictionary(didx);
    while(d != nullptr) {
        size_t diidx = 0;
        const DictionaryItem* di = d->GetFirstDictionaryItem(diidx);
        uint16_t dictionaryId = d->GetId();

        while(di != nullptr) {
         
            di = d->GetNextDictionaryItem(diidx);
            expressions_.push_back(di->GetRegex().c_str());
            uint16_t termId = di->GetId();
            uint16_t dictId = di->GetId();
// TODO Make id from dictionary id and term id &
            DictionaryTermId(dictId, termId);
            ids_.push_back(di->GetId());
            
               
        }

        d = ds->GetNextDictionary(didx);
    }
}

}
