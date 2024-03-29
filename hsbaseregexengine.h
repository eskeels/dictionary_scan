#pragma once

#include "regexengine.h"
#include "dictionaries.h"

namespace DLP {
class HSBaseRegexEngine : public RegexEngine {
    public:
        virtual ~HSBaseRegexEngine();
        virtual void RegisterDictionaryItem(uint16_t dictionaryId, const IDictionaryItem* di) = 0;
        virtual bool Initialize() = 0;
        virtual bool Initialize(char *bytes, size_t length, std::string& errDesc);
        bool Serialize(char **bytes, size_t *length, std::string& errDesc) = 0;
        virtual unsigned GetFlags(const IDictionaryItem* di) = 0;
        virtual IRegexScanState* CreateRegexScanState() const;
        hs_database_t* GetDatabase() { return database_; }

    protected:
        const Dictionaries* dictionaries_;
        std::vector<const char *> expressions_;
        std::vector<unsigned> flags_;
        std::vector<unsigned> ids_;
        std::vector<size_t> lens_;
        hs_database_t *database_ = nullptr;
};
}
