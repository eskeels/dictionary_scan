#pragma once

#include <stdint.h>
#include <string>
#include <vector>

#include "idictionaryitem.h"

namespace DLP
{

enum class ItemType { Term, TermPartial, Phrase, Regex, ProximityTerm, ProximityRegex }; 

class DictionaryItem : public IDictionaryItem {
    public:
        DictionaryItem(int16_t score,
                       bool distinct,
                       bool partial,
                       bool caseSensitive,
                       uint16_t id,
                       std::string regex,
                       uint16_t proximityId) :
                score_(score),
                distinct_(distinct),
                partial_(partial),
                caseSensitive_(caseSensitive),
                id_(id),
                regex_(regex),
                proximityId_(proximityId) {}

        ~DictionaryItem() {}

        uint16_t GetId() const { return id_; }
        const std::string& GetRegex() const { return regex_; }
        int16_t GetScore() const { return score_; }
        bool IsDistinct() const { return distinct_; }
        bool IsCaseSensitive() const { return caseSensitive_; }
        bool IsLiteral() const { return false; }
        bool IsPartial() const { return partial_; }
        bool IsProximity() const { return proximityId_ != 0; }
        uint16_t GetProximityId() const { return proximityId_; }
        uint16_t GetVerificationId() const = 0; 

    protected:
        int16_t score_;
        bool distinct_;
        bool partial_;
        bool caseSensitive_;
        uint16_t id_;
        std::string regex_;
        uint16_t proximityId_;
};

class Literal : public DictionaryItem {
    public:
        Literal(int16_t score,
                bool distinct,
                bool partial,
                bool caseSensitive,
                uint16_t id,
                std::string regex,
                uint16_t proximity) : DictionaryItem(score,distinct,partial,caseSensitive,id,regex,proximity)
        {
        }

        bool IsLiteral() const { return true; }
        uint16_t GetVerificationId() const { return 0; } 
};

class Regex : public DictionaryItem {
    public:
        Regex(int16_t score,
              bool distinct,
              bool partial,
              bool caseSensitive,
              uint16_t id,
              uint16_t verificationId,
              std::string regex,
              uint16_t proximity) : DictionaryItem(score,distinct,partial,caseSensitive,id,regex,proximity)
        {
            verificationId_ = verificationId;
        }

        uint16_t GetVerificationId() const { return verificationId_; } 

    protected:
        uint16_t verificationId_;

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

