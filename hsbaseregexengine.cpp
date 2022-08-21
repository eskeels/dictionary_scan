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

    bool HSBaseRegexEngine::Initialize(char *bytes, size_t length, std::string& errDesc) {
        hs_error_t hsErr = hs_deserialize_database(bytes, length, &database_);
        if (hsErr != HS_SUCCESS) {
            errDesc = "de-serialize failed!";
            std::cout << errDesc << std::endl;
        } else {
            std::cout << "de-serialize worked" << std::endl;
        }
        return true;
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

