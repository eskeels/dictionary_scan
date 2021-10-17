#pragma once

#include <stdint.h>
#include <string>
#include <vector>

namespace DLP
{

enum class ItemType { Term, Phrase, Regex, Proximity }; 

class DictionaryItem {
    public:
        DictionaryItem(ItemType type,
             int16_t score,
             bool distinct,
             uint16_t id,
             uint16_t valgoId,
             std::string regex) :
                type_(type),
                score_(score),
                distinct_(distinct),
                id_(id),
                valgoId_(valgoId),
                regex_(regex) {}

        uint16_t GetId() const { return id_; }
        const std::string& GetRegex() const { return regex_; }
        int16_t GetScore() const { return score_; }
        bool IsDistinct() const { return distinct_; }
    protected:
        ItemType type_;
        int16_t score_;
        bool distinct_;
        uint16_t id_;
        uint16_t valgoId_;
        std::string regex_;
};
}

