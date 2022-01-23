#pragma once

#include <stdint.h>
#include <string>
#include <vector>

#include "idictionaryitem.h"

namespace DLP
{

enum class ItemType { Term, TermPartial, Phrase, Regex, Proximity }; 

class DictionaryItem : public IDictionaryItem {
    public:
        DictionaryItem(int16_t score,
                       bool distinct,
                       bool partial,
                       bool caseSensitive,
                       uint16_t id,
                       std::string regex) :
                score_(score),
                distinct_(distinct),
                partial_(partial),
                caseSensitive_(caseSensitive),
                id_(id),
                regex_(regex) {}

        ~DictionaryItem() {}

        uint16_t GetId() const { return id_; }
        const std::string& GetRegex() const { return regex_; }
        int16_t GetScore() const { return score_; }
        bool IsDistinct() const { return distinct_; }
        bool IsCaseSensitive() const { return caseSensitive_; }
        bool IsLiteral() const { return false; }
        bool IsPartial() const { return partial_; }
    protected:
        int16_t score_;
        bool distinct_;
        bool partial_;
        bool caseSensitive_;
        uint16_t id_;
        std::string regex_;
};

class Literal : public DictionaryItem {
    public:
        Literal(int16_t score,
                bool distinct,
                bool partial,
                bool caseSensitive,
                uint16_t id,
                std::string regex) : DictionaryItem(score,distinct,partial,caseSensitive,id,regex)
        {
        }

        bool IsLiteral() const { return true; }
};

class Regex : public DictionaryItem {
    public:
        Regex(int16_t score,
              bool distinct,
              bool partial,
              bool caseSensitive,
              uint16_t id,
              uint16_t valgoId,
              std::string regex) : DictionaryItem(score,distinct,partial,caseSensitive,id,regex)
        {
            valgoId_ = valgoId;
        }

    protected:
        uint16_t valgoId_;

};
/*
class Proximity : public DictionaryItem {
    public:
        Literal(int16_t score,
                bool distinct,
                uint16_t id,
                std::string regex) : DictionaryItem(score,distinct,id,regex)
        {
        }
};
*/
}

