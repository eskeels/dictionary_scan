#include <iostream>
#include <string>
#include <codecvt>
#include <cassert>
#include <locale>

#include "normalizer.h"

#include "utf8proc.h"

namespace DLP {
char * normalize( const char* input,
                  size_t len,
                  bool stripAccents,
                  bool lowerCase,
                  bool lumpCharacters,
                  size_t& newLen ) {
    utf8proc_option_t opts = (utf8proc_option_t)(UTF8PROC_STABLE | UTF8PROC_DECOMPOSE | UTF8PROC_COMPAT | UTF8PROC_IGNORE );

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
    utf8proc_ssize_t normLen = utf8proc_map((const utf8proc_uint8_t *)input, (utf8proc_ssize_t)len, &retval, opts);
    newLen = normLen;
    return (char *)retval;
}
}

