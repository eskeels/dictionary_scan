#pragma once

#include "dictionaries.h"
#include "dictionaryitemfactory.h"
#include "json11.hpp"

namespace DLP {
    class DictionaryJSONParser {
        public:
            DictionaryJSONParser(const std::string& input)
                : input_(input)
            {}

            void Parse(Dictionaries& dictionaries); 

        protected:
            void ProcessFlags(const std::string& flags, bool& distinct, bool& partial, bool& caseSensitive) const;
            void ParseLiterals(const json11::Json& literals, DictionaryItemFactory& factory, Dictionary* dictionary); 
            void ParseLiterals(const json11::Json& literals, DictionaryItemFactory& factory, uint16_t proximityId, std::vector<const IDictionaryItem*>& items); 
            void ParseProximityLiterals(const json11::Json& literals, uint8_t distance, DictionaryItemFactory& factory, Dictionary* dictionary); 
            void ParseRegexes(const json11::Json& regexes, DictionaryItemFactory& factory, Dictionary* dictionary); 
            void ParseDictionaryAttributes(const json11::Json& jDic, std::string& name, uint16_t& dictionaryId, uint16_t& revision);

            const std::string& input_;
            uint16_t dictIdSeqGen_ = 101;  
    };
}
