#pragma once

#include "regexengine.h"
#include "dictionaries.h"

namespace DLP {
class HSRegexEngine : public RegexEngine {
    public:
        void RegisterDictionaryItem(uint16_t dictionaryId, const DictionaryItem* di);
        void Initialize();

    protected:
        const Dictionaries* dictionaries_;
        std::vector<const char *> expressions_;
        std::vector<unsigned> flags_;
        std::vector<unsigned> ids_;
};
}
