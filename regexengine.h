#pragma once

#include "iregexengine.h"

namespace DLP {

class Dictionaries;

class RegexEngine : public IRegexEngine {
    public:
        virtual void Register(const Dictionaries* ds);
        virtual void RegisterDictionaryItem(uint16_t dictionaryId, const DictionaryItem* di) = 0;
        virtual void Initialize() = 0;
        virtual ~RegexEngine(){};
};

}
