#pragma once

#include <set>

#include "dictionaries.h"

namespace DLP {
class IScanMatches {
    public:
        virtual ~IScanMatches() {};
};

class Match {
    public:
        Match(const IDictionaryItem* dictionaryItem, unsigned long long from, unsigned long long to)
            : dictionaryItem_(dictionaryItem),
              from_(from),
              gotFrom_(true),
              to_(to)
        {
        }

        Match(const IDictionaryItem* dictionaryItem, unsigned long long to)
            : dictionaryItem_(dictionaryItem),
              from_(0),
              gotFrom_(false),
              to_(to)
        {
        }

        size_t GetFrom() const {
            return (size_t) from_;
        }
        
        bool HasFrom() const {
            return gotFrom_;
        }

        size_t GetTo() const {
            return (size_t) to_;
        }

    protected:
        const IDictionaryItem* dictionaryItem_;
        unsigned long long from_;
        bool gotFrom_ = false;
        unsigned long long to_;
};

class DictionaryScanMatches : public IScanMatches {
    public:
        DictionaryScanMatches(const Dictionaries* dictionaries);
        ~DictionaryScanMatches();

        void SetInputBuffer(const char* input, size_t len);
        void RecordMatch(uint16_t dictionaryId, uint16_t itemId, unsigned long long from, unsigned long long to);
        void RecordMatch(uint16_t dictionaryId, uint16_t itemId, unsigned long long to);

        Match* GetFirstMatch(size_t& idx) {
            idx = 0;
            return GetNextMatch(idx);
        }

        Match* GetNextMatch(size_t& idx) {
            if (idx >= GetMatchCount()){
                return nullptr;
            }

            Match* ret = &matches_[idx];
            ++idx;
            return ret;
        }

        size_t GetMatchCount() const {
            return matches_.size();
        }

        Match* GetFirstMatch(size_t& idx, uint16_t did) {
            idx = 0;
            return GetNextMatch(idx, did);
        }

        Match* GetNextMatch(size_t& idx, uint16_t did) {
            auto it = dictionariesMatchIndx_.find(did);
            if (it == dictionariesMatchIndx_.end()) {
                return 0;
            }

            if (idx >= it->second.size()) {
                return nullptr;
            }
            Match* ret = &(matches_[it->second[idx]]);
            ++idx;
            return ret;
        }

        size_t GetMatchCount(uint16_t did) const {
            auto it = dictionariesMatchIndx_.find(did);
            if (it == dictionariesMatchIndx_.end()) {
                return 0;
            }
            return it->second.size();
        }

        const std::set<uint16_t>& GetMatchedDictionaryIds() const {
            return  matchedDictionaryIds_;
        }

    protected:
        void UpdateDictionariesMatchIndex(Match&& match, uint16_t dictionaryId);

        const Dictionaries* dictionaries_;
        const char* input_;
        size_t len_;
        std::vector<Match> matches_;
        // dictionary id to offset in matches_ index. So we can
        // find all matches for a dictionary
        std::unordered_map<uint16_t, std::vector<uint32_t>> dictionariesMatchIndx_;
        // dictionary ids that have matched
        std::set<uint16_t> matchedDictionaryIds_;
};

}
