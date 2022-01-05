#include <iostream>

#include "dictionaryscanmatches.h"

namespace DLP {

DictionaryScanMatches::DictionaryScanMatches(const Dictionaries* dictionaries)
    : dictionaries_(dictionaries),
      score_(0) {
}

DictionaryScanMatches::~DictionaryScanMatches() {
}

void DictionaryScanMatches::SetInputBuffer(const char* input, size_t len) {
    input_ = input;
    len_ = len;
}

int64_t DictionaryScanMatches::GetScore(uint16_t dictionaryId) const {
    auto it = dictionaryScores_.find(dictionaryId);
    if (it != dictionaryScores_.end()) {
        return it->second;
    }

    return 0;
} 

void DictionaryScanMatches::RecordScore(const Match& match, uint16_t dictionaryId) {
     score_ += match.GetScore();
   
auto it = dictionaryScores_.find(dictionaryId);
    if (it == dictionaryScores_.end()) {
        // insert
        dictionaryScores_.insert(std::make_pair(dictionaryId,match.GetScore()));
    } else {
        // update
        it->second += match.GetScore();
    }
}

void DictionaryScanMatches::RecordMatch(Match&& match, uint16_t dictionaryId) {
    RecordScore(match, dictionaryId);
    matchedDictionaryIds_.insert(dictionaryId);

    matches_.push_back(match);
    uint32_t idx = (uint32_t)matches_.size()-1;
    auto it = dictionariesMatchIndx_.find(dictionaryId);
    if (it == dictionariesMatchIndx_.end()) {
        // not found so create
        dictionariesMatchIndx_.insert({dictionaryId,{(uint32_t)idx}});
    } else {
        it->second.push_back((uint32_t)idx);
    }
}

void DictionaryScanMatches::RecordMatch(uint16_t dictionaryId, uint16_t itemId, unsigned long long from, unsigned long long to) {
    std::cout << dictionaryId << " " << itemId << " " << from << " " << to << std::endl;

    const IDictionaryItem* dictionaryItem = dictionaries_->GetDictionaryItem(dictionaryId, itemId);
    if (dictionaryItem) {
        RecordMatch(Match(dictionaryItem, from, to),dictionaryId);
    } else {
        // TODO: Trying to add a match with no corresponding entry in dictionary
        // raise error / warning
    }
}

void DictionaryScanMatches::RecordMatch(uint16_t dictionaryId, uint16_t itemId, unsigned long long to) {
    std::cout << dictionaryId << " " << itemId << " " << " " << to << std::endl;

    const IDictionaryItem* dictionaryItem = dictionaries_->GetDictionaryItem(dictionaryId, itemId);
    if (dictionaryItem) {
        RecordMatch(Match(dictionaryItem, to), dictionaryId);
    } else {
        // TODO: Trying to add a match with no corresponding entry in dictionary
        // raise error / warning
    }
}

}
