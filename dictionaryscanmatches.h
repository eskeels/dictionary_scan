#pragma once

#include <set>
#include <unordered_map>

#include "dictionaries.h"

namespace DLP {
class IScanMatches {
    public:
        virtual ~IScanMatches() {};
        virtual void ResolveProximity() = 0;
};

class Match {
    public:
        Match(const IDictionaryItem* dictionaryItem, uint16_t dictionaryId, unsigned long long from, unsigned long long to, uint8_t context)
            : dictionaryItem_(dictionaryItem),
              dictionaryId_(dictionaryId),
              from_(from),
              gotFrom_(true),
              to_(to),
              context_(context)
        {
        }

        Match(const IDictionaryItem* dictionaryItem, uint16_t dictionaryId, unsigned long long to, uint8_t context)
            : dictionaryItem_(dictionaryItem),
              dictionaryId_(dictionaryId),
              from_(0),
              gotFrom_(false),
              to_(to),
              context_(context)
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

        int16_t GetScore() const {
            return dictionaryItem_->GetScore();
        }

        int16_t GetItemId() const {
            return dictionaryItem_->GetId();
        }

        bool IsDistinct() const {
            return dictionaryItem_->IsDistinct();
        }

        uint8_t GetContext() const {
            return context_;
        }

        bool IsPartial() const {
            return dictionaryItem_->IsPartial();
        }

        bool IsProximity() const {
            return dictionaryItem_->IsProximity();
        }

        uint16_t GetProximityId() const {
            return dictionaryItem_->GetProximityId();
        }

        uint16_t GetVerificationId() const {
            return dictionaryItem_->GetVerificationId();
        }

        uint16_t GetDictionaryId() const {
            return dictionaryId_;
        }

        bool IsProximityMatch(const Match& rhs, uint8_t distance) const {
            // ignore existing matches
            if (IsProximityMatch() || rhs.IsProximityMatch()) {
                std::cout << "Ignore existing prox" << std::endl;
                return false;
            }

            // match distance
            unsigned long long md = 0;
            if (this->to_ > rhs.to_) {
                md = this->to_ - rhs.to_;
            } else {
                md = rhs.to_ - this->to_;
            }
   
            if (md <= distance) {
                return true;
            }
            std::cout << "md is " << md << " distance is " << (int)distance << std::endl;
            return false;    
        }

        void SetProximityMatch() {
            proximityMatch_ = true;
        }

        bool IsProximityMatch() const {
            return proximityMatch_;
        }
    
        bool IsDisabled() const {
            return disabled_;
        }

        void DisableMatch() {
            disabled_ = true;
        } 
    protected:
        const IDictionaryItem* dictionaryItem_;
        uint16_t dictionaryId_;
        unsigned long long from_;
        bool gotFrom_ = false;
        unsigned long long to_;
        uint8_t context_;
        bool proximityMatch_ = false;
        bool disabled_ = false;
};

class DictionaryScanMatches : public IScanMatches {
    public:
        DictionaryScanMatches(const Dictionaries* dictionaries);
        ~DictionaryScanMatches();

        void ResolveProximity();
        void SetInputBuffer(const char* input, size_t len);
        void SetContext(uint8_t context);
        void SetOffset(size_t offset);
        void SetOverlap(size_t overlap);
        void SetMatchLimit(size_t matchLimit);
        void RecordMatch(uint16_t dictionaryId, uint16_t itemId, unsigned long long from, unsigned long long to);
        void RecordMatch(uint16_t dictionaryId, uint16_t itemId, unsigned long long to);

        // TODO: Cater for disabled matches
        Match* GetFirstMatch(size_t& idx) {
            idx = 0;
            return GetNextMatch(idx);
        }

        // TODO: Cater for disabled matches
        Match* GetNextMatch(size_t& idx) {
            if (idx >= GetMatchCount()){
                return nullptr;
            }

            Match* ret = &matches_[idx];
            ++idx;
            return ret;
        }

        size_t GetMatchCount() const {
            return matches_.size() - disabledMatches_;
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

        // TODO: Cater for disabled matches
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

        void GetMatchedDictionaryNames(std::vector<std::string>& names) const {
            for (uint16_t id : matchedDictionaryIds_) {
                names.push_back(dictionaries_->GetDictionary(id)->GetName());
            }
        }

        int64_t GetTotalScore() const {
            return score_;
        }

        int64_t GetScore(uint16_t dictionaryId) const; 

        void CreateMatchSnippets(const std::set<uint16_t>& dictionaryIds, bool all, size_t count, size_t affix, std::vector<std::string>& snippets, uint8_t* pcontext = nullptr);
    protected:
        void RecordMatch(Match&& match, uint16_t dictionaryId);
        void RecordScore(const Match& match, uint16_t dictionaryId);
        void RecordProximityScore(Match& m1, Match& m2, uint16_t dictionaryId);
        bool CheckDistinct(const Match& match, uint16_t dictionaryId);
        bool CheckOverlap(const Match& match, uint16_t dictionaryId);
        bool CheckPartial(const Match& match);
        bool CheckVerificationRoutine(const Match& match, uint16_t dictionaryId);

        std::string GetSnippet(const Match& match, size_t affix);

        const Dictionaries* dictionaries_;
        const char* input_ = nullptr;
        size_t len_ = 0;
        std::vector<Match> matches_;
//        std::vector<Match> proximityMatches_;
        // index into matches_
        std::vector<uint32_t> proxMatchIdx_;

        // dictionary id to offset in matches_ index. So we can
        // find all matches for a dictionary
        std::unordered_map<uint16_t, std::vector<uint32_t>> dictionariesMatchIndx_;
        // dictionary ids that have matched
        std::set<uint16_t> matchedDictionaryIds_;
        // dictionary id to proximity ids of matched proximities
        std::set<std::pair<uint16_t,uint16_t>> matchedProximities_;
        // total score for the scan
        int64_t score_ = 0;
        // dictionary id to score
        std::unordered_map<uint16_t,int64_t> dictionaryScores_; 
        // set of all distinct matches. Used to record distinct triggers. 
        // dictionary id <--> term id
        std::set<std::pair<uint16_t,uint16_t>> distinctMatches_;
        // context id
        uint8_t context_ = 0;
        // overlap when scanning in chunks
        size_t overlap_ = 0;
        // offset when scanning in chunks
        size_t offset_ = 0;
        // set of absolute offset / dictionary id / term id
        std::set<std::tuple<size_t,uint16_t,uint16_t>> matchesInOverlap_;
        // limit on number of matches
        size_t matchLimit_ = 100000;
        // whether there was any proximity matches
        bool haveProximityMatch_ = false;
        // count of disabled matches (total
        // accross all dictionaries)
        size_t disabledMatches_ = 0;
};
}
