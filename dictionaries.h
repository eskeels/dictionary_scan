#pragma once

#include "dictionary.h"

namespace DLP {
class Dictionaries {
    public:
        void Add(Dictionary&& d);
        const Dictionary* GetDictionary(const std::string& name) const;
        const Dictionary* GetDictionary(uint16_t id) const;

    protected:
        // all the dictionaries
        std::vector<Dictionary> dictionaries_;
        // dictionary id to offset in above vector        
        std::unordered_map<uint16_t,uint16_t> dictionariesIndx_;
        // dictionary name to offset
        std::unordered_map<std::string,uint16_t> dictionariesNameIndx_;
};
}
