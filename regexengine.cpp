#include <stdint.h>
#include <cstddef>
#include <iostream>

#include "regexengine.h"
#include "dictionaries.h"
#include "dictionarytermid.h"

namespace DLP {

    void RegexEngine::Register(const Dictionaries* ds) {
        size_t didx = 0;
        const Dictionary* d = ds->GetFirstDictionary(didx);
std::cout << "IN Regex Engine HERE" << std::endl;
        while(d != nullptr) {
            size_t diidx = 0;
            const DictionaryItem* di = d->GetFirstDictionaryItem(diidx);
            uint16_t dictionaryId = d->GetId();
std::cout << "ID " << dictionaryId << std::endl;
            while(di != nullptr) {
                RegisterDictionaryItem(dictionaryId, di);
                di = d->GetNextDictionaryItem(diidx);
            }
            d = ds->GetNextDictionary(didx);
        }
    }
}

