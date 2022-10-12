#include "gtest/gtest.h"
#include "utf8proc.h"
#include "normalizer.h"
#include "verificationfactory.h"
#include "ukni.h"

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

TEST (NormalizerTest, ascii) {
std::setlocale(LC_ALL, "en_US.utf8");
    //std::string s = "xjc 123 45 6Û  xx xxx";
    std::string s = "abc Jc1234 5  6  q pp";
    std::wstring out;
    const char* startPos = 0;
    IVerification* ukni = VerificationFactory::CreateVerification(1);
    std::cout << ukni->Verify(&s[0], s.size(), 17, out, startPos) << std::endl;
    std::cout << wstring_to_utf8(out) << std::endl;
    std::cout << "Start pos is " << *startPos << std::endl;
}

