#include "gtest/gtest.h"

#include "normalizer.h"

using namespace DLP;

TEST (NormalizerTest, ascii) {
    std::string s = "the cat sat";
    size_t newLen;
    char * result = normalize(&s[0], s.size(), true, true, true, newLen);
    std::string sresult(result);
    free(result);
    EXPECT_EQ(sresult, s);
}

TEST (NormalizerTest, math) {
    std::string s = "𝐀𝐁𝐃";
    size_t newLen;
    char * result = normalize(&s[0], s.size(), true, true, true, newLen);
    std::string sresult(result);
    free(result);
    EXPECT_EQ(sresult, "abd");
}

TEST (NormalizerTest, japaneseFullWidth) {
    std::string s = "！＃＄";
    size_t newLen;
    char * result = normalize(&s[0], s.size(), true, true, true, newLen);
    std::string sresult(result);
    free(result);
    EXPECT_EQ(sresult, "!#$");
}

// Test strip accents  á
// lower case

