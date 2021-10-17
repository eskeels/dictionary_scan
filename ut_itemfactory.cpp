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
