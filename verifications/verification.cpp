#include <cwctype>

#include "verification.h"
#include "utf8proc.h"

namespace DLP {

std::wstring Verification::GetVerificationTerm(const char* input, 
                                                  size_t /*inputLen*/, 
                                                  size_t matchEndPos,
                                                  size_t minTermLen,
                                                  size_t maxTermLen,
                                                  bool dropSpace,
                                                  bool dropPunct,
                                                  const char*& matchStartPos) const {
    std::wstring matchText(maxTermLen,'\0');
    const char* p = input + matchEndPos;
    // set here incase we don't assign to it later
    matchStartPos = p;
    int mtPos = maxTermLen - 1;
    size_t termSize = 0;
    while (p >= input && mtPos >= 0) {
        // see if byte start 10
        if (!((*p & 0xC0) == 0x80)) {
            // record p. This will eventually be
            // the start of the match
            matchStartPos = p;
            // TODO: -1 will read up to 4 bytes, make sure that isnt off the end
            utf8proc_int32_t cp = 0;
            if (utf8proc_iterate((const utf8proc_uint8_t*)p, 4, &cp) <= 0) {
                // error so default cp to *p
                cp = *p;
            }
            bool keep = true;
            if (std::iswcntrl(cp)) {
                keep = false;
            }
            if (keep && dropSpace && std::iswspace(cp)) {
                keep = false;
            }
            if (keep && dropPunct && std::iswpunct(cp)) {
                keep = false;
            }
            if (keep) {
                matchText[mtPos] = std::towupper((wchar_t)cp);
                mtPos--;
                termSize++;
            }
        }
        p--;
    }

    if (termSize < minTermLen) {
        matchText = L"";
    } else if (termSize < maxTermLen && mtPos+1 >= 0) {
        matchText = matchText.substr(mtPos);
    }

    return matchText;
}

}

