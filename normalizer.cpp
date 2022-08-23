#include <iostream>
#include <string>
#include <codecvt>
#include <cassert>
#include <locale>

#include "normalizer.h"

#include "utf8proc.h"

struct Traits {
    utf8proc_int32_t previousCodepoint = 0;
    bool newline2Space = false;
    bool tabs2Space = false;
    bool compressSpace = false;
};

// data points to int32_t containing previous character
// different functions
//      + newline -> space
//		+ tab + vertical tab-> space
//		+ compress spaces (including converted)
//		+ compress newline (if not converting to space)
// Newline2Space - LF + CRLF -> space
// Tabs2Space - convert tabs to space
// CompressSpace - if current char is space and previous char is space emit 8203
// CompressNewLine - Convert CRLF to LF. If previous current is LF same as previous emit 8203
utf8proc_int32_t custFunc(utf8proc_int32_t codepoint, void *data) {
    Traits& normTraits = *(Traits*)data;
    utf8proc_int32_t newCodepoint = codepoint;
    if (normTraits.newline2Space && 
        (codepoint == '\n' || codepoint == '\r')) {
       newCodepoint = ' ';
    }
    if (normTraits.tabs2Space && 
        (codepoint == '\t' || codepoint == '\v')) {
       newCodepoint = ' ';
    }
    normTraits.previousCodepoint = codepoint;
    return newCodepoint;
}

namespace DLP {
std::string normalize( const char* input,
                  size_t len,
                  bool stripAccents,
                  bool lowerCase,
                  bool lumpCharacters,
                  size_t& newLen,
                  bool newline2Space,
                  bool tabs2Space,
                  bool compressSpace) {
    utf8proc_option_t opts = (utf8proc_option_t)(UTF8PROC_NLF2LF | UTF8PROC_STABLE | UTF8PROC_DECOMPOSE | UTF8PROC_COMPAT | UTF8PROC_IGNORE );

    if (lumpCharacters) {
        opts = (utf8proc_option_t) (opts | UTF8PROC_LUMP);
    }

    if (stripAccents) {
        opts = (utf8proc_option_t) (opts | UTF8PROC_STRIPMARK);
    }

    if (lowerCase) {
        opts = (utf8proc_option_t) (opts | UTF8PROC_CASEFOLD);
    }

    utf8proc_uint8_t *retval;
//    utf8proc_ssize_t normLen = utf8proc_map((const utf8proc_uint8_t *)input, (utf8proc_ssize_t)len, &retval, opts);

    Traits normTraits;
    normTraits.newline2Space = newline2Space;
    normTraits.tabs2Space = tabs2Space;
    normTraits.compressSpace = compressSpace;

    utf8proc_ssize_t normLen = utf8proc_map_custom((const utf8proc_uint8_t *)input, (utf8proc_ssize_t)len, &retval, opts, &custFunc, &normTraits);

    newLen = normLen;

    if (newLen == 0 || retval == nullptr) {
        if (retval) {
            free (retval);
        }
        return std::string();
    }

    if (compressSpace) {
        char * pc = (char *)retval;
        std::string retStr(1, *pc);
        pc++;
        for (size_t i = 1; i < newLen; ++i) {
            if (!(retStr[retStr.size()-1] == ' ' && *pc == ' ')) {
                retStr.append(1,*pc);
            }
            pc++;
        }
        free (retval);
        return retStr;
    } else {
        std::string retStr((char *)retval, newLen);
        free (retval);
        return retStr;
    }
}
}

