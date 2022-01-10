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

    DLP::DictionaryScanMatches dsm = Scan(ds, "shouldnt get any triggers");
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

    EXPECT_EQ(true, ScanAndVerify2(ds,
                                  "goat the duck",
                                  {{0,4,1},   // goat - animals
                                   {0,4,2},   // goat - also animals
                                   {9,13,1},  // duck - animals 
                                   {9,13,2}}  // duck - also animals
                                   ));
}
