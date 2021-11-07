#pragma once

#include "dictionaries.h"

namespace DLP {
class RegexEngine {
    public:
        void Initialise(const Dictionaries* ds);

    protected:
        const Dictionaries* dictionaries_;
        std::vector<const char *> expressions_;
        std::vector<unsigned> flags_;
        std::vector<unsigned> ids_;
};
}
