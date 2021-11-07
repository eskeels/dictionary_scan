#include <iostream>

#include "dictionaries.h"

namespace DLP {

void Dictionaries::Add(Dictionary&& d) {
    std::string name = d.GetName();
    uint16_t id = d.GetId();

    dictionaries_.push_back(std::move(d));

    uint16_t pos = static_cast<uint16_t>(dictionaries_.size()) - 1;
    dictionariesIndx_.insert(std::make_pair(id, pos));
std::cout << "name is " << d.GetName() << std::endl;
    dictionariesNameIndx_.insert(std::make_pair(name, pos));
}

const Dictionary* Dictionaries::GetDictionary(const std::string& name) const {
    auto pos = dictionariesNameIndx_.find(name);

    if (pos == dictionariesNameIndx_.end()) {
        return nullptr;
    }

    return &dictionaries_[pos->second];
}

const Dictionary* Dictionaries::GetNextDictionary(size_t& idx) const {
    if (idx >= dictionaries_.size()){
        return nullptr;
    }

    const Dictionary* ret = &dictionaries_[idx];
    ++idx;
    return ret;
}

}
