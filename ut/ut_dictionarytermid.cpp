#include "gtest/gtest.h"

#include "dictionarytermid.h"

TEST (DictionaryTermId, Test13) {
    DLP::DictionaryTermId dti(1,3);
    DLP::DictionaryTermId dti2(dti.GetId());
    EXPECT_EQ(1, dti2.GetDictionaryId());
    EXPECT_EQ(3, dti2.GetTermId());
}

TEST (DictionaryTermId, Test00) {
    DLP::DictionaryTermId dti(0,0);
    DLP::DictionaryTermId dti2(dti.GetId());
    EXPECT_EQ(0, dti2.GetDictionaryId());
    EXPECT_EQ(0, dti2.GetTermId());
}

TEST (DictionaryTermId, Test9999) {
    DLP::DictionaryTermId dti(99,99);
    DLP::DictionaryTermId dti2(dti.GetId());
    EXPECT_EQ(99, dti2.GetDictionaryId());
    EXPECT_EQ(99, dti2.GetTermId());
}

TEST (DictionaryTermId, Test10244096) {
    DLP::DictionaryTermId dti(1024,4096);
    DLP::DictionaryTermId dti2(dti.GetId());
    EXPECT_EQ(1024, dti2.GetDictionaryId());
    EXPECT_EQ(4096, dti2.GetTermId());
}

