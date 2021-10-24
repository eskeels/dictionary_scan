#pragma once

#include "dictionaryitem.h"

namespace DLP {

class DictionaryItemFactory {
    public:
        DictionaryItemFactory() : 
            defaultScore_(0),
            defaultDistinct_(false),
            itemId_(0),
            defaultPartial_(false),
            defaultCaseSensitive_(false),
            defaultDistance_(0){}

        void SetDefaults (int16_t score,
                          bool distinct,
                          bool partial,
                          bool caseSensitive,
                          uint8_t distance) {
            defaultScore_       = score;
            defaultDistinct_    = distinct;
            defaultPartial_     = partial;
            defaultCaseSensitive_ = caseSensitive;
            defaultDistance_    = distance; 
        }

        void ResetId() {
            itemId_ = 0;
        }
        
        uint16_t GetId() {
            itemId_++;
            return itemId_;
        }
 
        DictionaryItem CreateTerm(const std::string& term, int16_t* score, bool* distinct, bool* partial, bool* caseSensitive);
        DictionaryItem CreatePhrase(int16_t* score, bool* distinct, bool* caseSensitive, uint8_t distance, std::vector<std::string> terms);
        DictionaryItem CreateRegex(int16_t* score, bool* distinct);
        DictionaryItem CreateProximity(int16_t* score, bool* distinct, std::vector<std::string> terms);
                 
    protected:
        int16_t defaultScore_;
        bool defaultDistinct_;
        uint16_t itemId_;
        bool defaultPartial_;
        bool defaultCaseSensitive_;
        uint8_t defaultDistance_;
};

}

