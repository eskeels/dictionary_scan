#include <cwctype>

#include "ukni.h"

namespace DLP {

bool isDFIQUV(wchar_t c) {
    switch(c) {
        case L'D':
        case L'F':
        case L'I':
        case L'Q':
        case L'U':
        case L'V':
            return true;
        default:
            break;
    }
    return false;
}


bool UKNI::Verify(const char* input, size_t inputLen, size_t matchEndPos, std::wstring& out, const char*& matchStartPos) const {
    const size_t NI_LEN = 9;
    out = GetVerificationTerm(input, inputLen, matchEndPos, NI_LEN, NI_LEN, true, true, matchStartPos);

    bool bOk = false;
    if (!out.empty()) {
        
        if (isalpha(out[0]) && isalpha(out[1])) {
            bool has6Digits = true;
            for (int i = 2 ; i <= 7 && has6Digits; ++i) {
                 if (!isdigit(out[i])) {
                    has6Digits = false;
                }
            }
            if (has6Digits && iswalpha(out[8])) {
                bOk = true;
            }
            if (bOk && (isDFIQUV(out[0]) || isDFIQUV(out[1]))) {
                bOk = false;
            }
            if (bOk && out[1] == L'O') {
                bOk = false;
            }
        }
    }

    return bOk;
}

}
