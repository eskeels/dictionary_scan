#include "regexengine.h"

namespace DLP {

void RegexEngine::Initialise(const Dictionaries* ds) {
    size_t didx = 0;
    const Dictionary* d = ds->GetFirstDictionary(didx);
    while(d != nullptr) {
        size_t diidx = 0;
        const DictionaryItem* di = d->GetFirstDictionaryItem(diidx);
        while(di != nullptr) {
        
            di = d->GetNextDictionaryItem(diidx);
        }

        d = ds->GetNextDictionary(didx);
    }
}

}
