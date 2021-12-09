#include <iostream>

#include "dictionary.h"

namespace DLP {

Dictionary::~Dictionary() {
    for( auto di : items_ ){
        delete di;
    }
}
void Dictionary::Add(const IDictionaryItem* item) {
    items_.push_back(item);
    uint16_t pos = static_cast<uint16_t>(items_.size()) - 1;
    itemsIndx_.insert(std::make_pair(item->GetId(), pos));
}

const IDictionaryItem* Dictionary::GetDictionaryItem(uint16_t id) const {
    auto pos = itemsIndx_.find(id);

    if (pos == itemsIndx_.end()) {
        return nullptr;
    }

    return items_[pos->second];
}

const IDictionaryItem* Dictionary::GetNextDictionaryItem(size_t& idx) const {
    if (idx >= items_.size()){
        return nullptr;
    }

    const IDictionaryItem* ret = items_[idx];
    ++idx;
    return ret;
}

}
