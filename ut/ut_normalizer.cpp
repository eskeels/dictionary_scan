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
    char * result = normalize(&s[0], s.size(), true, false, true, newLen);
    std::string sresult(result);
    free(result);
    EXPECT_EQ(sresult, "ABD");
}

TEST (NormalizerTest, japaneseFullWidth) {
    std::string s = "！＃＄";
    size_t newLen;
    char * result = normalize(&s[0], s.size(), true, true, true, newLen);
    std::string sresult(result);
    free(result);
    EXPECT_EQ(sresult, "!#$");
}

TEST (NormalizerTest, stripAccents) {
    std::string s = "àáăǎ";
    size_t newLen;
    char * result = normalize(&s[0], s.size(), true, true, true, newLen);
    std::string sresult(result);
    free(result);
    EXPECT_EQ(sresult, "aaaa");
}

TEST (NormalizerTest, lowerCase) {
    std::string s = "UPPERCASE";
    size_t newLen;
    char * result = normalize(&s[0], s.size(), true, true, true, newLen);
    std::string sresult(result);
    free(result);
    EXPECT_EQ(sresult, "uppercase");
}
