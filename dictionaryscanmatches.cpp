#include <iostream>

#include "dictionaryscanmatches.h"

namespace DLP {

DictionaryScanMatches::DictionaryScanMatches(const Dictionaries* dictionaries)
    : dictionaries_(dictionaries) {
}

DictionaryScanMatches::~DictionaryScanMatches() {
}

void DictionaryScanMatches::SetInputBuffer(const char* input, size_t len) {
    input_ = input;
    len_ = len;
}

void DictionaryScanMatches::UpdateDictionariesMatchIndex(Match&& match, uint16_t dictionaryId) {
    matches_.push_back(match);
    matchedDictionaryIds_.insert(dictionaryId);
    uint32_t idx = (uint32_t)matches_.size()-1;
    auto it = dictionariesMatchIndx_.find(dictionaryId);
    if (it == dictionariesMatchIndx_.end()) {
        std::cout << "Inserting" << std::endl;
        // not found so create
        dictionariesMatchIndx_.insert({dictionaryId,{(uint32_t)idx}});
    } else {
        std::cout << "Adding" << std::endl;
        it->second.push_back((uint32_t)idx);
    }
}

void DictionaryScanMatches::RecordMatch(uint16_t dictionaryId, uint16_t itemId, unsigned long long from, unsigned long long to) {
    std::cout << dictionaryId << " " << itemId << " " << from << " " << to << std::endl;

    const IDictionaryItem* dictionaryItem = dictionaries_->GetDictionaryItem(dictionaryId, itemId);
    if (dictionaryItem) {
//        matches_.push_back(Match(dictionaryItem, from, to));
        UpdateDictionariesMatchIndex(Match(dictionaryItem, from, to),dictionaryId);
    } else {
        // TODO: Trying to add a match with no corresponding entry in dictionary
        // raise error / warning
    }
}

void DictionaryScanMatches::RecordMatch(uint16_t dictionaryId, uint16_t itemId, unsigned long long to) {
    std::cout << dictionaryId << " " << itemId << " " << " " << to << std::endl;

    const IDictionaryItem* dictionaryItem = dictionaries_->GetDictionaryItem(dictionaryId, itemId);
    if (dictionaryItem) {
//        matches_.push_back(Match(dictionaryItem, to));
        UpdateDictionariesMatchIndex(Match(dictionaryItem, to), dictionaryId);
    } else {
        // TODO: Trying to add a match with no corresponding entry in dictionary
        // raise error / warning
    }
}

}
