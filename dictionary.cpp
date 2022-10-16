#include <iostream>

#include "dictionary.h"

namespace DLP {

Dictionary::~Dictionary() {
}

uint16_t Dictionary::Add(const IDictionaryItem* item) {
    items_.push_back(std::move(std::unique_ptr<const IDictionaryItem>(item)));
    uint16_t pos = static_cast<uint16_t>(items_.size()) - 1;
    itemsIndx_.insert(std::make_pair(item->GetId(), pos));
    return pos;
}

void Dictionary::AddLiteralProximity(std::vector<const IDictionaryItem*> items, uint8_t distance) {
    distance = distance;
    std::vector<uint16_t> itemIds;
    for (auto di : items) {
        if (!di->IsProximity()) {
            std::cerr << "ERROR: Proximity flag not set!" << std::endl;
        }
        itemIds.push_back(Add(di));
    }
    proximityItems_.insert(std::make_pair(proximityCount_, itemIds));
    proximityDistance_.insert(std::make_pair(proximityCount_, distance));
    ++proximityCount_; 
}

const IDictionaryItem* Dictionary::GetDictionaryItem(uint16_t id) const {
    auto pos = itemsIndx_.find(id);

    if (pos == itemsIndx_.end()) {
        return nullptr;
    }

    return items_[pos->second].get();
}

const IDictionaryItem* Dictionary::GetNextDictionaryItem(size_t& idx) const {
    if (idx >= items_.size()){
        return nullptr;
    }

    const IDictionaryItem* ret = items_[idx].get();
    ++idx;
    return ret;
}

}
