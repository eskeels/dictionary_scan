#include <iostream>
#include <memory>
#include "dictionaries.h"

namespace DLP {

Dictionaries::~Dictionaries() {
}

void Dictionaries::Add(Dictionary* d) {
    std::string name = d->GetName();
    uint16_t id = d->GetId();

    dictionaries_.push_back(std::move(std::unique_ptr<const Dictionary>(d)));

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

    return dictionaries_[pos->second].get();
}

const Dictionary* Dictionaries::GetDictionary(uint16_t id) const {
    auto pos = dictionariesIndx_.find(id);

    if (pos == dictionariesIndx_.end()) {
        return nullptr;
    }

    return dictionaries_[pos->second].get();
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

    const Dictionary* ret = dictionaries_[idx].get();
    ++idx;
    return ret;
}

}
