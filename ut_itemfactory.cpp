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

TEST (DictionaryItemFactoryTest, TestPhrase3Terms) {
    DictionaryItemFactory dif;
    bool caseSensitive = true;
    uint8_t distance = 5;

    // Case sensitive and partial
    {
        caseSensitive = true;
        DictionaryItem di = dif.CreatePhrase(nullptr, // score
                                             nullptr, // distinct
                                             &caseSensitive,
                                             distance,                    
                                             { "the", "cat", "sat"} );
        EXPECT_EQ(std::string("\\bthe[\\W\\n]{1,5}cat[\\W\\n]{1,5}sat\\b"), di.GetRegex());
        EXPECT_EQ(true, std::regex_search("the cat sat",std::regex(di.GetRegex())));
        EXPECT_EQ(true, std::regex_search("the? cat,    sat",std::regex(di.GetRegex())));

    }
}

TEST (DictionaryItemFactoryTest, TestPhrase2Terms) {
    DictionaryItemFactory dif;
    bool caseSensitive = true;
    uint8_t distance = 5;

    // Case sensitive and partial
    {
        caseSensitive = true;
        DictionaryItem di = dif.CreatePhrase(nullptr, // score
                                             nullptr, // distinct
                                             &caseSensitive,
                                             distance,                    
                                             { "the", "cat"} );
        EXPECT_EQ(std::string("\\bthe[\\W\\n]{1,5}cat\\b"), di.GetRegex());
    }
}

TEST (DictionaryItemFactoryTest, TestPhrase1Term) {
    DictionaryItemFactory dif;
    bool caseSensitive = true;
    uint8_t distance = 5;

    // Case sensitive and partial
    {
        caseSensitive = true;
        DictionaryItem di = dif.CreatePhrase(nullptr, // score
                                             nullptr, // distinct
                                             &caseSensitive,
                                             distance,                    
                                             { "the"} );
        EXPECT_EQ(std::string("\\bthe\\b"), di.GetRegex());
    }
}

TEST (DictionaryItemFactoryTest, TestPhrase0Term) {
    DictionaryItemFactory dif;
    bool caseSensitive = true;
    uint8_t distance = 5;

    // Case sensitive and partial
    {
        caseSensitive = true;
        DictionaryItem di = dif.CreatePhrase(nullptr, // score
                                             nullptr, // distinct
                                             &caseSensitive,
                                             distance,                    
                                             { } );
        EXPECT_EQ(std::string("\\b\\b"), di.GetRegex());
    }
}

TEST (DictionaryItemFactoryTest, TestPhrase2TermsCaseInsensitive) {
    DictionaryItemFactory dif;
    bool caseSensitive = false;
    uint8_t distance = 5;

    // Case insensitive and partial
    {
        caseSensitive = false;
        DictionaryItem di = dif.CreatePhrase(nullptr, // score
                                             nullptr, // distinct
                                             &caseSensitive,
                                             distance,                    
                                             { "the", "cat"} );
        EXPECT_EQ(std::string("(?i)\\bthe[\\W\\n]{1,5}cat\\b"), di.GetRegex());
    }
}

TEST (DictionaryItemFactoryTest, TestPhrase2TermsCaseSensitive) {
    DictionaryItemFactory dif;
    bool caseSensitive = false;
    uint8_t distance = 5;

    // Case sensitive and partial
    {
        caseSensitive = true;
        DictionaryItem di = dif.CreatePhrase(nullptr, // score
                                             nullptr, // distinct
                                             &caseSensitive,
                                             distance,                    
                                             { "THE", "cat"} );
        EXPECT_EQ(std::string("\\bTHE[\\W\\n]{1,5}cat\\b"), di.GetRegex());
        EXPECT_EQ(true, std::regex_search(" THE?! cat,??    sat",std::regex(di.GetRegex())));
    }
}

TEST (DictionaryItemFactoryTest, TestProximity2TermsCaseSensitive) {
    DictionaryItemFactory dif;
    bool caseSensitive = false;
    uint8_t distance = 5;

    // Case sensitive and partial
    {
        caseSensitive = true;
        DictionaryItem di = dif.CreateProximity(nullptr, // score
                                                nullptr, // distinct
                                                &caseSensitive,
                                                distance,                    
                                                { "THE", "cat"} );
        EXPECT_EQ(std::string("\\b(THE|cat)[\\W\\n]{1,5}(THE|cat)\\b"), di.GetRegex());
    }
}
