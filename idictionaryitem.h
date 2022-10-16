#pragma once

#include <stdint.h>
#include <string>
#include <vector>

namespace DLP
{

class IDictionaryItem {
    public:
        virtual uint16_t GetId() const = 0;
        virtual const std::string& GetRegex() const = 0;
        virtual int16_t GetScore() const = 0; 
        virtual bool IsDistinct() const = 0; 
        virtual bool IsCaseSensitive() const = 0;
        virtual bool IsLiteral() const = 0;
        virtual bool IsPartial() const = 0;
        virtual bool IsProximity() const = 0;
        virtual uint16_t GetProximityId() const = 0;
        virtual uint16_t GetVerificationId() const = 0; 
        virtual ~IDictionaryItem() {};
};
}

