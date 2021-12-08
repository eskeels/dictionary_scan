#pragma once

#include "regexengine.h"
#include "dictionaries.h"

namespace DLP {
class HSBaseRegexEngine : public RegexEngine {
    public:
        void RegisterDictionaryItem(uint16_t dictionaryId, const DictionaryItem* di);
        void Initialize() = 0;
        void Serialize() = 0; 
        virtual unsigned GetFlags(const DictionaryItem* di) = 0;
    protected:
        const Dictionaries* dictionaries_;
        std::vector<const char *> expressions_;
        std::vector<unsigned> flags_;
        std::vector<unsigned> ids_;
        std::vector<size_t> lens_;
        hs_database_t *database_ = nullptr;


};
}
