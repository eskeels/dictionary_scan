#include <iostream>

#include "dictionaries.h"

namespace DLP {

Dictionaries::~Dictionaries() {
    for (auto d : dictionaries_) {
        delete d;
    }
}

void Dictionaries::Add(const Dictionary* d) {
    std::string name = d->GetName();
    uint16_t id = d->GetId();

    dictionaries_.push_back(d);

    uint16_t pos = static_cast<uint16_t>(dictionaries_.size()) - 1;
    dictionariesIndx_.insert(std::make_pair(id, pos));
std::cout << "name is " << d->GetName() << std::endl;
    dictionariesNameIndx_.insert(std::make_pair(name, pos));
}

const Dictionary* Dictionaries::GetDictionary(const std::string& name) const {
    auto pos = dictionariesNameIndx_.find(name);

    if (pos == dictionariesNameIndx_.end()) {
        return nullptr;
    }

    return dictionaries_[pos->second];
}

const Dictionary* Dictionaries::GetDictionary(uint16_t id) const {
    auto pos = dictionariesIndx_.find(id);

    if (pos == dictionariesIndx_.end()) {
        return nullptr;
    }

    return dictionaries_[pos->second];
}

const IDictionaryItem* Dictionaries::GetDictionaryItem(uint16_t dictionaryId, uint16_t itemId) const {
    const Dictionary* dictionary = GetDictionary(dictionaryId);
    if (dictionary) {
        return dictionary->GetDictionaryItem(itemId);
    }

    return nullptr;
}
 
const Dictionary* Dictionaries::GetNextDictionary(size_t& idx) const {
    if (idx >= dictionaries_.size()){
        return nullptr;
    }

    const Dictionary* ret = dictionaries_[idx];
    ++idx;
    return ret;
}

}
