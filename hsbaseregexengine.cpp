#include <iostream>

#include <hs.h>

#include "hsbaseregexengine.h"
#include "dictionarytermid.h"
#include "hsregexscanstate.h"

namespace DLP {
    HSBaseRegexEngine::~HSBaseRegexEngine() {
        hs_free_database(database_);
    }

    IRegexScanState* HSBaseRegexEngine::CreateRegexScanState() const {
        std::cout << __LINE__ << std::endl;
        HSRegexScanState* rss = new HSRegexScanState(dictionaries_);
        std::cout << __LINE__ << std::endl;

        rss->Initialize(database_);
        std::cout << __LINE__ << std::endl;
        return rss;
    }
}

