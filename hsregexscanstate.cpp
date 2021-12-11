#include <iostream>

#include <hs.h>

#include "hsregexscanstate.h"
#include "dictionarytermid.h"

namespace DLP {
    HSRegexScanState::HSRegexScanState(const Dictionaries* dictionaries) 
        : dictionaries_(dictionaries)
    {
    }

    void HSRegexScanState::Initialize(const hs_database_t *database) {
        std::cout << "DB " << database << " scratch " << scratch_ << std::endl;
        if (hs_alloc_scratch(database, &scratch_) != HS_SUCCESS) {
            fprintf(stderr, "ERROR: Unable to allocate scratch space. Exiting.\n");
        }
    }

    HSRegexScanState::~HSRegexScanState() {
        hs_free_scratch(scratch_);
    }
}

