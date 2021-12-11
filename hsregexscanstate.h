#pragma once

#include "iregexscanstate.h"
#include "dictionaries.h"
#include <hs.h>

namespace DLP {
class HSRegexScanState : public IRegexScanState {
    public:
        HSRegexScanState(const Dictionaries* dictionaries);
        virtual ~HSRegexScanState();
        void Initialize(const hs_database_t *database);

    protected:
        const Dictionaries* dictionaries_;
        hs_scratch_t* scratch_ = nullptr;
};
}
