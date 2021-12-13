#include <iostream>

#include <hs.h>

#include "litregexengine.h"
#include "dictionarytermid.h"
#include "dictionaryitem.h"

namespace DLP {
    LitRegexEngine::~LitRegexEngine() {
    }

    void LitRegexEngine::RegisterDictionaryItem(uint16_t dictionaryId, const IDictionaryItem* di) {
        if (di->IsLiteral()) {
            itemCount_++;
            expressions_.push_back(di->GetRegex().c_str());
            uint16_t termId = di->GetId();
            // each id holds the dictionary id and term id
            DictionaryTermId dti(dictionaryId, termId);
            ids_.push_back(dti.GetId());
            flags_.push_back(GetFlags(di));
            lens_.push_back(di->GetRegex().size());
        }
    }


    void LitRegexEngine::Initialize() {
        if (expressions_.size()>0) {
            hs_compile_error_t *compile_err;
            if (hs_compile_lit_multi(&expressions_[0],
                                     &flags_[0],
                                     &ids_[0],
                                     &lens_[0],
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
    }

    void LitRegexEngine::Serialize() {
        char *bytes;
        size_t length;
        if (HS_SUCCESS == hs_serialize_database(database_, &bytes, &length)) {
            std::cout << "Length is " << length << std::endl;
        } else {
            std::cout << "FAIL!" << std::endl;
        }
    }
}

