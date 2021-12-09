#pragma once

#include <stdint.h>

namespace DLP {

class Dictionaries;
class IDictionaryItem;

class IRegexEngine {
    public:
        virtual void Register(const Dictionaries* ds) = 0;
        virtual void RegisterDictionaryItem(uint16_t dictionaryId, const IDictionaryItem* di) = 0;
        virtual void Initialize() = 0;
        virtual void Serialize() = 0;
        virtual ~IRegexEngine(){};
};

}
