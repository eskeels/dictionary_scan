#include <iostream>

#include <hs.h>

#include "hsregexengine.h"
#include "dictionarytermid.h"

namespace DLP {

    void HSRegexEngine::Initialize() {
std::cout << expressions_[0] << std::endl << expressions_[1] << std::endl;
        hs_compile_error_t *compile_err;
        if (hs_compile_multi(&expressions_[0],
                             &flags_[0],
                             &ids_[0],
                             expressions_.size(),
                             HS_MODE_BLOCK,
                             NULL,
                             &database_,
                             &compile_err) != HS_SUCCESS) {
            fprintf(stderr, "ERROR: Unable to compile: %s\n", compile_err->message);
            hs_free_compile_error(compile_err);
            return;
        }
    }

    void HSRegexEngine::Serialize() {
        char *bytes;
        size_t length;
        if (HS_SUCCESS == hs_serialize_database(database_, &bytes, &length)) {
            std::cout << "Length is " << length << std::endl;
        } else {
            std::cout << "FAIL!" << std::endl;
        }
    }
}

