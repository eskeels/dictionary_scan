#pragma once

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

    protected:
        const Dictionaries* dictionaries_;
        const char* input_;
        size_t len_;
        std::vector<Match> matches_;
};

}
