#include <iostream>

#include "dictionary.h"

namespace DLP {

void Dictionary::Add(DictionaryItem&& item) {
    items_.push_back(std::move(item));
    uint16_t pos = static_cast<uint16_t>(items_.size()) - 1;
    itemsIndx_.insert(std::make_pair(item.GetId(), pos));
}

const DictionaryItem* Dictionary::GetDictionaryItem(uint16_t id) const {
    auto pos = itemsIndx_.find(id);

    if (pos == itemsIndx_.end()) {
        return nullptr;
    }

    return &items_[pos->second];
}

const DictionaryItem* Dictionary::GetNextDictionaryItem(size_t& idx) const {
    if (idx >= items_.size()){
        return nullptr;
    }

    const DictionaryItem* ret = &items_[idx];
    ++idx;
    return ret;
}

}
