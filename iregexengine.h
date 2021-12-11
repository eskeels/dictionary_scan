#pragma once

#include <stdint.h>

namespace DLP {

class Dictionaries;
class IDictionaryItem;
class IRegexScanState;

class IRegexEngine {
    public:
        virtual void Register(const Dictionaries* ds) = 0;
        virtual void RegisterDictionaryItem(uint16_t dictionaryId, const IDictionaryItem* di) = 0;
        virtual void Initialize() = 0;
        virtual void Serialize() = 0;
        virtual ~IRegexEngine(){};
        virtual IRegexScanState* CreateRegexScanState() const = 0;
};

}
