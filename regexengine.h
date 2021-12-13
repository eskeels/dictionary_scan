#pragma once

#include "iregexengine.h"

namespace DLP {

class Dictionaries;
class IDictionaryItem;

class RegexEngine : public IRegexEngine {
    public:
        virtual void Register(const Dictionaries* ds);
        virtual ~RegexEngine(){};
        virtual void RegisterDictionaryItem(uint16_t dictionaryId, const IDictionaryItem* di) = 0;
        virtual void Initialize() = 0;
        virtual void Serialize() = 0;
        virtual IRegexScanState* CreateRegexScanState() const = 0;
        size_t GetItemCount() const { return itemCount_; }
    protected:
        size_t itemCount_ = 0;        
};

}
