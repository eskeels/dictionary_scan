#include <regex>

#include "gtest/gtest.h"

#include "dictionaryitemfactory.h"

using namespace DLP;

bool verifyRegex( const std::string& regex, const std::string& txt, bool trigger ) {
    std::string r(regex);
    std::regex::flag_type f(std::regex_constants::ECMAScript);
    if (regex.substr(0,4) == "(?i)") {
        r = regex.substr(4);
        f |= std::regex_constants::icase;
    }
    std::regex rgx(r,f);
    return (trigger == std::regex_search(txt, rgx));
}

TEST (DictionaryItemFactoryTest, AllDefault) {
    DictionaryItemFactory dif;
    DictionaryItem di = dif.CreateTerm("term", nullptr, nullptr, nullptr, nullptr);
    EXPECT_EQ(di.GetRegex(), std::string("\\bterm\\b"));
    EXPECT_TRUE(verifyRegex(di.GetRegex(), "  term  ", true));
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
        EXPECT_TRUE(verifyRegex(di.GetRegex(), "xxtermxx", true));
        EXPECT_TRUE(verifyRegex(di.GetRegex(), "xxtrmxx", false));
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
        EXPECT_TRUE(verifyRegex(di.GetRegex(), "xxtermxx", false));
        EXPECT_TRUE(verifyRegex(di.GetRegex(), "x term xx", true));

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
        EXPECT_TRUE(verifyRegex(di.GetRegex(), "xxtermxx", true));
        EXPECT_TRUE(verifyRegex(di.GetRegex(), "x term xx", true));
        EXPECT_TRUE(verifyRegex(di.GetRegex(), "x TeRM xx", true));
    }
    // Case insensitive and not partial
    {
        partial = false; caseSensitive = false;
        DictionaryItem di = dif.CreateTerm( "term", 
                                            nullptr, // score
                                            nullptr, // distinct
                                            &partial,
                                            &caseSensitive );
        EXPECT_EQ(di.GetRegex(), std::string("(?i)\\bterm\\b"));
        EXPECT_TRUE(verifyRegex(di.GetRegex(), "x term xx", true));
        EXPECT_TRUE(verifyRegex(di.GetRegex(), "x termxx", false));
        EXPECT_TRUE(verifyRegex(di.GetRegex(), "x TERM xx", true));
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
        EXPECT_TRUE(verifyRegex(di.GetRegex(), "the cat sat", true));
        EXPECT_TRUE(verifyRegex(di.GetRegex(), "the? cat,    sat", true));
    }
}

TEST (DictionaryItemFactoryTest, TestPhrase2Terms) {
    DictionaryItemFactory dif;
    bool caseSensitive = true;
    uint8_t distance = 5;

    // Case sensitive
    {
        caseSensitive = true;
        DictionaryItem di = dif.CreatePhrase(nullptr, // score
                                             nullptr, // distinct
                                             &caseSensitive,
                                             distance,                    
                                             { "the", "cat"} );
        EXPECT_EQ(std::string("\\bthe[\\W\\n]{1,5}cat\\b"), di.GetRegex());
        EXPECT_TRUE(verifyRegex(di.GetRegex(), "the? cat,    sat", true));
        EXPECT_TRUE(verifyRegex(di.GetRegex(), "the? CAT,    sat", false));
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

    // Case insensitive
    {
        caseSensitive = false;
        DictionaryItem di = dif.CreatePhrase(nullptr, // score
                                             nullptr, // distinct
                                             &caseSensitive,
                                             distance,                    
                                             { "the", "cat"} );
        EXPECT_EQ(std::string("(?i)\\bthe[\\W\\n]{1,5}cat\\b"), di.GetRegex());
        EXPECT_TRUE(verifyRegex(di.GetRegex(), "the CAT    sat", true));
        EXPECT_TRUE(verifyRegex(di.GetRegex(), "the? CAT    sat", true));
        EXPECT_TRUE(verifyRegex(di.GetRegex(), "the CAT,    sat", true));
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
        EXPECT_EQ(false, std::regex_search(" THE?!    cat,??    sat",std::regex(di.GetRegex())));
    }
}

TEST (DictionaryItemFactoryTest, TestProximity3TermsCaseSensitive) {
    DictionaryItemFactory dif;
    bool caseSensitive = false;
    uint8_t distance = 5;

    {
        caseSensitive = true;
        DictionaryItem di = dif.CreateProximity(nullptr, // score
                                                nullptr, // distinct
                                                &caseSensitive,
                                                distance,                    
                                                { "THE", "cat","sat"} );
        EXPECT_TRUE(verifyRegex(di.GetRegex(), " THE?! cat,??    sat", true));
    }
}

