#pragma once

#include <stdint.h>

#include <string>

namespace DLP {

class Dictionaries;
class IDictionaryItem;
class IRegexScanState;

class IRegexEngine {
    public:
        virtual void Register(const Dictionaries* ds) = 0;
        virtual void RegisterDictionaryItem(uint16_t dictionaryId, const IDictionaryItem* di) = 0;
        virtual void Initialize() = 0;
        virtual bool Initialize(char *bytes, size_t length, std::string& errDesc) = 0;
        virtual bool Serialize(char **bytes, size_t *length, std::string& errDesc) = 0;
        virtual ~IRegexEngine(){};
        virtual IRegexScanState* CreateRegexScanState() const = 0;
        virtual size_t GetItemCount() const = 0;

};

}
