#include "gtest/gtest.h"

#include "normalizer.h"

using namespace DLP;

TEST (NormalizerTest, ascii) {
    std::string s = "the cat sat";
    size_t newLen;
    std::string sresult = normalize(&s[0], s.size(), true, true, true, newLen);
    EXPECT_EQ(sresult, s);
}

TEST (NormalizerTest, math) {
    std::string s = "𝐀𝐁𝐃";
    size_t newLen;
    std::string sresult = normalize(&s[0], s.size(), true, false, true, newLen);
    EXPECT_EQ(sresult, "ABD");
}

TEST (NormalizerTest, japaneseFullWidth) {
    std::string s = "！＃＄";
    size_t newLen;
    std::string sresult = normalize(&s[0], s.size(), true, true, true, newLen);
    EXPECT_EQ(sresult, "!#$");
}

TEST (NormalizerTest, stripAccents) {
    std::string s = "àáăǎ";
    size_t newLen;
    std::string sresult = normalize(&s[0], s.size(), true, true, true, newLen);
    EXPECT_EQ(sresult, "aaaa");
}

TEST (NormalizerTest, lowerCase) {
    std::string s = "UPPERCASE";
    size_t newLen;
    std::string sresult = normalize(&s[0], s.size(), true, true, true, newLen);
    EXPECT_EQ(sresult, "uppercase");
}

TEST (NormalizerTest, crlf2lf) {
    std::string s = "the\r\ncat";
    size_t newLen;
    std::string sresult = normalize(&s[0], s.size(), true, true, true, newLen);
    EXPECT_EQ(sresult, "the\ncat");
}

TEST (NormalizerTest, compressSpace) {
    std::vector<std::string> testStrings {"the cat sat ", "the  cat   sat ", "the        cat      sat                                "};
    for (auto s : testStrings) { 
        size_t newLen;
        std::string sresult = normalize(&s[0], s.size(), true, true, true, newLen, false, false, true);
        EXPECT_EQ(sresult, "the cat sat ");
    }
}

TEST (NormalizerTest, newline2space) {
    std::string s = "the\ncat\r\n";
    size_t newLen;
    std::string sresult = normalize(&s[0], s.size(), true, true, true, newLen, true);
    EXPECT_EQ(sresult, "the cat  ");
}

TEST (NormalizerTest, tab2space) {
    std::string s = "the\tcat\t\t";
    size_t newLen;
    std::string sresult = normalize(&s[0], s.size(), true, true, true, newLen, true, true);
    EXPECT_EQ(sresult, "the cat  ");
}

TEST (NormalizerTest, full) {
    std::vector<std::string> testStrings {"\nthe\tcat\tsat ",
                                          "\rthe\v\t  cat   sat ",
                                          " the    \v\t\n    cat\r\n      sat                                "};
    for (auto s : testStrings) { 
        size_t newLen;
        std::string sresult = normalize(&s[0], s.size(), true, true, true, newLen, true, true, true);
        EXPECT_EQ(sresult, " the cat sat ");
    }
}


