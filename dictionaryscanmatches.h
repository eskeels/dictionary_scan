#pragma once

#include "dictionaries.h"

namespace DLP {
class IScanMatches {
    public:
        virtual ~IScanMatches() {};
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
};

}
