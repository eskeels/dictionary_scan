#include <memory>
#include <regex>

#include "gtest/gtest.h"

#include "hsregexengine.h"
#include "litregexengine.h"
#include "dictionaryitemfactory.h"
#include "dictionaryscanner.h"
#include "dictionaryscanmatches.h"

#include "dictionaryscannerhelper.h"

using namespace DLP;

TEST (DictionaryEmpty, AllDefault) {
    DLP::Dictionaries ds;

    DLP::DictionaryScanMatches dsm = Scan(ds, "shouldnt get any triggers", 1);
    EXPECT_EQ(size_t(0), dsm.GetMatchCount());
    EXPECT_EQ(int64_t(0), dsm.GetTotalScore());
}

TEST (DictionaryOneItem, AllDefault) {
    DLP::Dictionaries ds;
    AddDictionary( ds, "words", 1 , { "sat" }, LITERAL );

    int64_t totalScore = 0;
    EXPECT_EQ(true, ScanAndVerify3(ds, "the cat sat on the mat", {{8,11,1}}, totalScore));
    EXPECT_EQ(20, totalScore);
}

TEST (DictionaryTwoItems, AllDefault) {
    DLP::Dictionaries ds;
    AddDictionary( ds, "words", 1 , { "sat", "mat" }, LITERAL );

    EXPECT_EQ(true, ScanAndVerify2(ds,
                                  "the cat sat on the mat",
                                  {{8,11,1},  // sat
                                   {19,22,1}} // mat
                                   ));
}

TEST (DictionaryOneItemTwoTriggers, AllDefault) {
    DLP::Dictionaries ds;
    AddDictionary( ds, "words", 1 , { "the" }, LITERAL );

    EXPECT_EQ(true, ScanAndVerify2(ds,
                                  "the cat sat on the mat",
                                  {{0,3,1},   // the
                                   {15,18,1}} // the
                                   ));
}

TEST (DictionaryOneItemRegex, AllDefault) {
    DLP::Dictionaries ds;
    AddDictionary( ds, "regexes", 1 , { R"(\d\d)" }, REGEX );

    EXPECT_EQ(true, ScanAndVerify2(ds, "the 22 cat sat on the mat 11", {{0,6,1},{0,28,1}}));
}

TEST (DictionaryTwoLiteralDictionariesThreeTriggers, AllDefault) {
    DLP::Dictionaries ds;
    AddDictionary( ds, "animals", 1 , { "duck","frog","goat" }, LITERAL );
    AddDictionary( ds, "weapons", 2 , { "axe","knife","hammer" }, LITERAL );

    EXPECT_EQ(true, ScanAndVerify2(ds,
                                  "goat axe duck",
                                  {{0,4,1},   // goat
                                   {5,8,2},   // axe
                                   {9,13,1}}  // duck
                                   ));
}

TEST (DictionaryTwoLiteralDictionariesDuplicate, AllDefault) {
    DLP::Dictionaries ds;
    AddDictionary( ds, "animals", 1 , { "duck","frog","goat" }, LITERAL );
    AddDictionary( ds, "also animals", 2 , { "duck","badger","goat" }, LITERAL );

    DLP::DictionaryScanMatches dsm(&ds);
    std::string input("goat the duck");
    EXPECT_EQ(true, ScanAndVerify2(ds,
                                   input,
                                  {{0,4,1},   // goat - animals
                                   {0,4,2},   // goat - also animals
                                   {9,13,1},  // duck - animals 
                                   {9,13,2}}  // duck - also animals
                                   ,&dsm));

    std::set<uint16_t> dictionaryIds;
    std::vector<std::string> snippets;

    dsm.CreateMatchSnippets(dictionaryIds, true, 5 /* count */, 0 /* affix */, snippets);
    ASSERT_EQ(4UL, snippets.size());
    EXPECT_EQ("goat", snippets[0]);
    EXPECT_EQ("duck", snippets[1]);
    EXPECT_EQ("goat", snippets[2]);
    EXPECT_EQ("duck", snippets[3]);
}

TEST (DictionaryOneItemFull, Full) {
    DLP::Dictionaries ds;
    // partial is false so only whole word will match
    AddDictionary( ds, "words", 1 , { "sat" }, LITERAL, 20, false, false, true );

    {
    const std::string txt = "satthe cat sat on the matsat";
    DLP::DictionaryScanMatches dsm = Scan(ds, txt, 1);
    std::vector<std::string> snippets;
    dsm.CreateMatchSnippets({}, true, 10, 3, snippets, nullptr);
    ASSERT_EQ(1UL, snippets.size());
    EXPECT_EQ("at sat on", snippets[0]);
    }
    {
    const std::string txt = "sat the cat saton the matsat";
    DLP::DictionaryScanMatches dsm = Scan(ds, txt, 1);
    std::vector<std::string> snippets;
    dsm.CreateMatchSnippets({}, true, 10, 3, snippets, nullptr);
    ASSERT_EQ(1UL, snippets.size());
    EXPECT_EQ("sat th", snippets[0]);
    }
    {
    const std::string txt = "the cat saton the sat";
    DLP::DictionaryScanMatches dsm = Scan(ds, txt, 1);
    std::vector<std::string> snippets;
    dsm.CreateMatchSnippets({}, true, 10, 3, snippets, nullptr);
    ASSERT_EQ(1UL, snippets.size());
    EXPECT_EQ("he sat", snippets[0]);
    }
}

TEST (DictionaryOneItemDistinct, Distinct) {
    DLP::Dictionaries ds;
    // distinct is true so only one match should count
    AddDictionary( ds, "words", 1 , { "sat" }, LITERAL, 20, false, true, true );

    {
    const std::string txt = "the cat sat on the mat SaT sat";
    DLP::DictionaryScanMatches dsm = Scan(ds, txt, 1);
    std::vector<std::string> snippets;
    dsm.CreateMatchSnippets({}, true, 10, 3, snippets, nullptr);
    ASSERT_EQ(1UL, snippets.size());
    EXPECT_EQ("at sat on", snippets[0]);
    }
}

TEST (DictionaryOneItemCaseInsensitive, CaseInsensitive) {
    DLP::Dictionaries ds;
    AddDictionary( ds, "words", 1 , { "SAT" }, LITERAL, 20, false, false, false );

    {
    const std::string txt = "the cat SAT on the mat saT";
    DLP::DictionaryScanMatches dsm = Scan(ds, txt, 1);
    std::vector<std::string> snippets;
    dsm.CreateMatchSnippets({}, true, 10, 3, snippets, nullptr);
    ASSERT_EQ(2UL, snippets.size());
    EXPECT_EQ("at SAT on", snippets[0]);
    EXPECT_EQ("at saT", snippets[1]);
    }
}

TEST (DictionaryOneItemCaseInsensitiveDistinct, CaseInsensitiveDistinct) {
    DLP::Dictionaries ds;
    AddDictionary( ds, "words", 1 , { "SAT" }, LITERAL, 20, false, true, false );

    {
    const std::string txt = "the cat SAT on the mat saT";
    DLP::DictionaryScanMatches dsm = Scan(ds, txt, 1);
    std::vector<std::string> snippets;
    dsm.CreateMatchSnippets({}, true, 10, 3, snippets, nullptr);
    ASSERT_EQ(1UL, snippets.size());
    EXPECT_EQ("at SAT on", snippets[0]);
    }
}

TEST (DictionaryOneItemNegativeTotalScore, AllDefault) {
    DLP::Dictionaries ds;
    AddDictionary( ds, "words", 1 , { "sat" }, LITERAL, -10 );

    int64_t totalScore = 0;
    EXPECT_EQ(true, ScanAndVerify3(ds, "the cat sat on the mat", {{8,11,1}}, totalScore));
    EXPECT_EQ(-10, totalScore);
}

TEST (DictionaryOneItemZeroTotalScore, AllDefault) {
    DLP::Dictionaries ds;
    AddDictionary( ds, "words1", 1 , { "sat" }, LITERAL, -10 );
    AddDictionary( ds, "words2", 2 , { "the" }, LITERAL, 10 );

    int64_t totalScore = 0;
    EXPECT_EQ(false, ScanAndVerify3(ds, "the cat sat on the mat", {{0,3,2},{8,11,1}}, totalScore));
    EXPECT_EQ(0, totalScore);
}

