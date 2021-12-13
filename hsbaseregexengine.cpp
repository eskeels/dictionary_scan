#include <iostream>

#include <hs.h>

#include "hsbaseregexengine.h"
#include "dictionarytermid.h"
#include "hsregexscanstate.h"

namespace DLP {
    HSBaseRegexEngine::~HSBaseRegexEngine() {
        if (database_) {
            hs_free_database(database_);
        }
    }

    IRegexScanState* HSBaseRegexEngine::CreateRegexScanState() const {
        if (GetItemCount() > 0) {
            HSRegexScanState* rss = new HSRegexScanState(dictionaries_);
            rss->Initialize(database_);
            return rss;
        }

        return nullptr;
    }
}

