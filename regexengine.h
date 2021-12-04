#pragma once

#include "iregexengine.h"
#include "dictionaries.h"

namespace DLP {
class RegexEngine : public IRegexEngine {
    public:
        void Initialise(const Dictionaries* ds);

    protected:
        const Dictionaries* dictionaries_;
        std::vector<const char *> expressions_;
        std::vector<unsigned> flags_;
        std::vector<unsigned> ids_;
};
}
