#include "gtest/gtest.h"

#include "dictionaryitemfactory.h"

using namespace DLP;

TEST (DictionaryItemFactoryTest, AllDefault) {
    DictionaryItemFactory dif;
    DictionaryItem di = dif.CreateTerm("term", nullptr, nullptr, nullptr, nullptr);
    EXPECT_EQ(di.GetRegex(), std::string("\\bterm\\b"));
}

TEST (DictionaryItemFactoryTest, SetDefaults) {
    DictionaryItemFactory dif;
    dif.SetDefaults(99, true, true, false, 7);
    DictionaryItem di = dif.CreateTerm("term", nullptr, nullptr, nullptr, nullptr);
    EXPECT_EQ(di.GetRegex(), std::string("term"));
    EXPECT_EQ(di.GetScore(), 99);
    EXPECT_EQ(di.IsDistinct(), true);
}

TEST (DictionaryItemFactoryTest, TestRegex) {
    DictionaryItemFactory dif;
    bool partial = true;
    bool caseSensitive = true;
    // Case sensitive and partial
    {
        partial = true; caseSensitive = true;
        DictionaryItem di = dif.CreateTerm( "term", 
                                            nullptr, // score
                                            nullptr, // distinct
                                            &partial,
                                            &caseSensitive );
        EXPECT_EQ(di.GetRegex(), std::string("term"));
    }
    // Case sensitive and not partial
    {
        partial = false; caseSensitive = true;
        DictionaryItem di = dif.CreateTerm( "term", 
                                            nullptr, // score
                                            nullptr, // distinct
                                            &partial,
                                            &caseSensitive );
        EXPECT_EQ(di.GetRegex(), std::string("\\bterm\\b"));
    }
    // Case insensitive and partial
    {
        partial = true; caseSensitive = false;
        DictionaryItem di = dif.CreateTerm( "term", 
                                            nullptr, // score
                                            nullptr, // distinct
                                            &partial,
                                            &caseSensitive );
        EXPECT_EQ(di.GetRegex(), std::string("(?i)term"));
    }
    // Case sensitive and not partial
    {
        partial = false; caseSensitive = false;
        DictionaryItem di = dif.CreateTerm( "term", 
                                            nullptr, // score
                                            nullptr, // distinct
                                            &partial,
                                            &caseSensitive );
        EXPECT_EQ(di.GetRegex(), std::string("(?i)\\bterm\\b"));
    }
}
