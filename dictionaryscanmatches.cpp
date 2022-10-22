#include <iostream>

#include "dictionaryscanmatches.h"
#include "verificationfactory.h"

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

void DictionaryScanMatches::SetContext(uint8_t context) {
    context_ = context;
}

void DictionaryScanMatches::SetOffset(size_t offset) {
    offset_ = offset;
}

void DictionaryScanMatches::SetOverlap(size_t overlap) {
    overlap_ = overlap;
}

void DictionaryScanMatches::SetMatchLimit(size_t matchLimit) {
    matchLimit_ = matchLimit;
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
            distinctMatches_.insert(std::make_pair(dictionaryId,match.GetItemId()));
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
        // TODO: No from (regex) so just use 3 for now
        if (startPos > 3) {
            startPos -= 3;
        } else {
            startPos = 0;
        }
std::cout << "startPos is " << startPos << std::endl;

    }
    
    if (prefix < startPos) {
        startPos -= prefix;
        // TODO: If this is midway through a utf-8 sequence
    } else {
        startPos = 0;
    }

    if (endPos+suffix <= len_) {
        endPos += suffix;
        // TODO: If this is midway through a utf-8 sequence
    } else {
        endPos = len_;
    }

    std::string s(input_ + startPos, input_ + endPos);
    return s;
}

void DictionaryScanMatches::CreateMatchSnippets(const std::set<uint16_t>& dictionaryIds, bool all, size_t count, size_t affix, std::vector<std::string>& snippets, uint8_t* pcontext) {
    auto d = dictionariesMatchIndx_.begin();
    while (d != dictionariesMatchIndx_.end()) {
        uint16_t did = d->first;
        if (all || (dictionaryIds.find(did) != dictionaryIds.end())) {
            auto& matches = d->second;
            for (size_t i=0; (i <= count) && (i < matches.size()) && (snippets.size() <= count) ; ++i) {
                auto& match = matches_[matches[i]];
                if (pcontext == nullptr || (pcontext != nullptr && match.GetContext() == *pcontext)) {
                    std::string s = GetSnippet(match, affix);
                    if (!s.empty()) {
                        snippets.push_back(std::move(s));
                    }
                    std::cout << match.GetFrom() << " " <<  match.GetTo() << std::endl;
                }
            }
        }
        d++;
    }
}

bool DictionaryScanMatches::CheckOverlap(const Match& match, uint16_t dictionaryId) {
    if (match.GetTo() < overlap_ || match.GetTo() > (len_ - overlap_)) {
        size_t absPos = match.GetTo() + offset_;
        auto it = matchesInOverlap_.find(std::make_tuple(absPos, dictionaryId, match.GetItemId()));
        if (it != matchesInOverlap_.end()) {
            // dupe trigger so ignore
            return true;
        }
        matchesInOverlap_.insert(std::make_tuple(absPos, dictionaryId, match.GetItemId()));
    }
    return false;
}

bool DictionaryScanMatches::CheckPartial(const Match& match) {
    if (match.IsPartial()) {
        // partials can match anywhere
        return false;
    }
    size_t to = match.GetTo();
    if (to < len_ && input_) {
        // TODO: make correct for utf-8 
        char after = *(input_ + to);
        // TODO: support all whitespace including unicode
        if (after != ' ') {
            return true;
        }
    }

    if (!match.HasFrom()) {
        // no start position so we can not
        // determine partial status. Return
        // false so match is recorded regardless.
        return false;
    }
    size_t from = match.GetFrom();
    if (from > 0 && input_) {
        // TODO: make correct for utf-8
        --from;
        char before = *(input_ + from);
        // TODO: support all whitespace including unicode
        if (before != ' ') {
            return true;
        } else {
            return false;
        }
    } else {
        // match at start of stream
        return false;
    }
}

bool DictionaryScanMatches::CheckVerificationRoutine(const Match& match, uint16_t /*dictionaryId*/) {
    uint16_t vid = match.GetVerificationId();
    if (vid != 0) {
        IVerification* vp = VerificationFactory::CreateVerification(vid);
        std::wstring out;
        const char* matchStartPos;

        return vp->Verify(input_, len_, match.GetTo(), out, matchStartPos);
    // use match to get dictionaryItem and validationId
    // apply validation
//        size_t to = match.GetTo();
// use input_ and len_

    // validation starts from end
    // remember it's utf-8
    }
    return false;
}

void DictionaryScanMatches::RecordMatch(Match&& match, uint16_t dictionaryId) {
    if (CheckDistinct(match, dictionaryId)) {
        // match was distinct and we already have a hit
        return;
    }

    if (overlap_ > 0 && CheckOverlap(match, dictionaryId)) {
        // match was in an overlap and was a duplicate
        return;
    }

    if (CheckVerificationRoutine(match, dictionaryId)) {
        // match failed verification routine
        return;
    }

    if (CheckPartial(match)) {
        // match was non partial and had non whitespace
        // characters before / after the start / end
        return;
    }

    // Check for proximity
    if (match.IsProximity()) {
        // This should be a function
        std::cout << "Got proximity " << match.GetItemId() << std::endl;
        proximityMatches_.push_back(match);
        matchedProximities_.insert(std::make_pair(dictionaryId,match.GetProximityId()));
    } else {
        // ignore score for now
        RecordScore(match, dictionaryId);
    }
    matchedDictionaryIds_.insert(dictionaryId);

    // check match limit
    if (matches_.size() >= matchLimit_) {
        return;
    }
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
        RecordMatch(Match(dictionaryItem, dictionaryId, from, to, context_),dictionaryId);
    } else {
        // TODO: Trying to add a match with no corresponding entry in dictionary
        // raise error / warning
    }
}

void DictionaryScanMatches::RecordMatch(uint16_t dictionaryId, uint16_t itemId, unsigned long long to) {
//    std::cout << dictionaryId << " " << itemId << " " << " " << to << std::endl;

    const IDictionaryItem* dictionaryItem = dictionaries_->GetDictionaryItem(dictionaryId, itemId);
    if (dictionaryItem) {
        RecordMatch(Match(dictionaryItem, dictionaryId, to, context_), dictionaryId);
    } else {
        // TODO: Trying to add a match with no corresponding entry in dictionary
        // raise error / warning
    }
}

void DictionaryScanMatches::ResolveProximity() {
    if (!proximityMatches_.empty()) {
       // need to find all the matches for the same proximityId and dictionaryId

        // - Iterate  matchedProximities_
        for(auto mp : matchedProximities_) {
            std::vector<Match> relatedMatches;
            const uint16_t dictionaryId = mp.first;
            const uint16_t proximityId = mp.second;
            // - for each pair, iterate proximityMatches_ and collate a list
            for (auto proxMatch : proximityMatches_) {
                //   of matches for that dictionary / proximity
                if (proxMatch.GetDictionaryId() == dictionaryId 
                    && proxMatch.GetProximityId() == proximityId) {
                    std::cout << "Adding match " << &proxMatch << " " << proxMatch.GetDictionaryId() << " " << proxMatch.GetItemId() << std::endl;
                    relatedMatches.push_back(proxMatch);
                }
            }
            auto dict = dictionaries_->GetDictionary(dictionaryId);
            uint8_t distance = 0;
            if (dict) {
                distance = dict->GetProximityDistance(proximityId);
            }
            std::cout << "Distance is: " << (int)distance << std::endl;
            for (auto m : relatedMatches) {
                std::cout << "Match: dictionary Id " << m.GetDictionaryId() << " Prox id " << m.GetProximityId() << std::endl;
                std::cout << "Match id " << m.GetItemId() << std::endl;
            }

            for (size_t i = 0; i < relatedMatches.size(); ++i) {
                for (size_t j = 0; j < relatedMatches.size(); ++j) {
                    if (i==j) {
                        continue;
                    }
// TODO: Skip match if disabled
        std::cout <<"comparing " << i << " to " << j << std::endl;
                    if (relatedMatches[i].IsProximityMatch(relatedMatches[j], distance)) {
                        std::cout << "Got proximity match" << std::endl;
                        RecordScore(relatedMatches[i], dictionaryId);
                        RecordScore(relatedMatches[j], dictionaryId);
// TODO: Disable match
                        break;
                    } else {
                        std::cout << "Proximity DIDNT match" << std::endl;
                    }
                }
            }
        }
        // - any leftover matches should be removed from match list (or reset to avoid re-alloc)
    }
}

}
