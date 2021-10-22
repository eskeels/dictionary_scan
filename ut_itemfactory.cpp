#include <regex>

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
        EXPECT_EQ(true, std::regex_search("xxtermxx",std::regex(di.GetRegex())));
        EXPECT_EQ(false, std::regex_search("xxtrmxx",std::regex(di.GetRegex())));

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
        EXPECT_EQ(false, std::regex_search("xxtermxx",std::regex(di.GetRegex())));
        EXPECT_EQ(true, std::regex_search("x term xx",std::regex(di.GetRegex())));

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

TEST (DictionaryItemFactoryTest, TestPhrase) {
    DictionaryItemFactory dif;
    bool caseSensitive = true;
    // Case sensitive and partial
    {
        caseSensitive = true;
        DictionaryItem di = dif.CreatePhrase(nullptr, // score
                                             nullptr, // distinct
                                             &caseSensitive,
                                             { "the", "cat", "sat"} );
        std::cout << di.GetRegex() << std::endl;
    }
}

