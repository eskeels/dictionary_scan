#include "gtest/gtest.h"
#include "utf8proc.h"
#include "normalizer.h"
#include <codecvt>
// convert UTF-8 string to wstring
std::wstring utf8_to_wstring (const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.from_bytes(str);
}

// convert wstring to UTF-8 string
std::string wstring_to_utf8 (const std::wstring& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.to_bytes(str);
}

using namespace DLP;

std::wstring GetVerificationTerm(const char* input, 
              size_t inputLen, 
              size_t matchEndPos,
              size_t minTermLen,
              size_t maxTermLen,
              bool dropSpace,
              bool dropPunct) {
    std::wstring matchText(maxTermLen,'\0');
    const char* p = input + matchEndPos;
    int mtPos = maxTermLen - 1;
    int termSize = 0;
    while (p >= input && mtPos >= 0) {
        // see if byte start 10
        if (!((*p & 0xC0) == 0x80)) {
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

bool VerifyNI(const char* input, 
              size_t inputLen, 
              size_t matchEndPos,
              std::wstring& out) {
    const size_t NI_LEN = 9;
    out = GetVerificationTerm(input, inputLen, matchEndPos, NI_LEN, NI_LEN, true, true);

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

TEST (NormalizerTest, ascii) {
std::setlocale(LC_ALL, "en_US.utf8");
    //std::string s = "xjc 123 45 6Û  xx xxx";
    std::string s = "Jc1234 5  6  q pp";
    std::wstring out;
    std::cout << VerifyNI(&s[0], s.size(), 13, out) << std::endl;
    std::cout << wstring_to_utf8(out) << std::endl;
}

