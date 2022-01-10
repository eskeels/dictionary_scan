#include <iostream>

#include "dictionaryscanmatches.h"

namespace DLP {

DictionaryScanMatches::DictionaryScanMatches(const Dictionaries* dictionaries)
    : dictionaries_(dictionaries),
      score_(0){
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
 
bool DictionaryScanMatches::CheckDistinct(const Match& match, uint16_t dictionaryId) {
    if (match.IsDistinct()) {
        auto it = distinctMatches_.find(std::make_pair(dictionaryId,match.GetItemId()));
        if (it == distinctMatches_.end()) {
            // insert
            distinctMatches_.insert(std::make_pair(dictionaryId,match.GetScore()));
            // distinct but first hit
            return false;
         } else {
            // distinct and already in set
            return true;
         }
    }
    // not distinct so just return
    return false;
}

void DictionaryScanMatches::RecordScore(const Match& match, uint16_t dictionaryId) {
    // update overall score
    score_ += match.GetScore();
    // update score for the dictionary 
    auto it = dictionaryScores_.find(dictionaryId);
    if (it == dictionaryScores_.end()) {
        // insert
        dictionaryScores_.insert(std::make_pair(dictionaryId,match.GetScore()));
    } else {
        // update
        it->second += match.GetScore();
    }
}

std::string DictionaryScanMatches::GetSnippet(const Match& match, size_t affix) {
    size_t startPos = 0;
    size_t endPos = match.GetTo();
    size_t prefix = affix;
    size_t suffix = affix;
    if (match.HasFrom()) {
        startPos = match.GetFrom();
    } else {
        startPos = match.GetTo();
    }
    
    if (prefix < startPos) {
        startPos -= prefix;
    } else {
        startPos = 0;
    }

    if (endPos+suffix <= len_) {
        endPos += suffix;
    } else {
        endPos = len_;
    }

    std::string s(input_ + startPos, input_ + endPos);
    return s;
}

void DictionaryScanMatches::CreateMatchSnippets(std::set<uint16_t> dictionaryIds, bool all, size_t count, size_t affix, std::vector<std::string>& snippets) {
    std::cout << "CreateMatchSnippets" << count << affix << std::endl;
    auto d = dictionariesMatchIndx_.begin();
    while (d != dictionariesMatchIndx_.end()) {
        uint16_t did = d->first;
        if (all || (dictionaryIds.find(did) != dictionaryIds.end())) {
            auto& matches = d->second;
            std::cout << "Dictionary id is " << did << std::endl;
            for (size_t i=0; (i <= count) && (i < matches.size()) && (snippets.size() <= count) ; ++i) {
                auto& match = matches_[matches[i]];
                std::string s = GetSnippet(match, affix);
                if (!s.empty()) {
                    std::cout << "Snip is [" << s << "]" << std::endl;
                    snippets.push_back(std::move(s));
                }
                std::cout << match.GetFrom() << " " <<  match.GetTo() << std::endl;
            }
        }
        d++;
    }
}

void DictionaryScanMatches::RecordMatch(Match&& match, uint16_t dictionaryId) {
    if (CheckDistinct(match, dictionaryId)) {
        // match was distinct and we already have a hit
        return;
    }

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
//    std::cout << dictionaryId << " " << itemId << " " << from << " " << to << std::endl;

    const IDictionaryItem* dictionaryItem = dictionaries_->GetDictionaryItem(dictionaryId, itemId);
    if (dictionaryItem) {
        RecordMatch(Match(dictionaryItem, from, to),dictionaryId);
    } else {
        // TODO: Trying to add a match with no corresponding entry in dictionary
        // raise error / warning
    }
}

void DictionaryScanMatches::RecordMatch(uint16_t dictionaryId, uint16_t itemId, unsigned long long to) {
//    std::cout << dictionaryId << " " << itemId << " " << " " << to << std::endl;

    const IDictionaryItem* dictionaryItem = dictionaries_->GetDictionaryItem(dictionaryId, itemId);
    if (dictionaryItem) {
        RecordMatch(Match(dictionaryItem, to), dictionaryId);
    } else {
        // TODO: Trying to add a match with no corresponding entry in dictionary
        // raise error / warning
    }
}

}
