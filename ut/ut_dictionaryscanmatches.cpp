#include "gtest/gtest.h"

#include "dictionaryitemfactory.h"
#include "dictionaryscanmatches.h"

TEST (DictionaryTermId, TestNoMatch) {
    DLP::Dictionaries ds;
    DLP::DictionaryItemFactory dif;
    {
        DLP::Dictionary* d = new DLP::Dictionary("A",1,1);
        d->Add(dif.CreateLiteral("aaa", nullptr, nullptr, nullptr, nullptr));
        ds.Add(d);
    }
    {
        dif.ResetId();
        DLP::Dictionary* d = new DLP::Dictionary("B",2,1);
        d->Add(dif.CreateLiteral("bbb1", nullptr, nullptr, nullptr, nullptr));
        d->Add(dif.CreateLiteral("bbb2", nullptr, nullptr, nullptr, nullptr));
        ds.Add(d);
    } 

    DLP::DictionaryScanMatches dsm(&ds);

    size_t idx;
    ASSERT_EQ(0,dsm.GetMatchCount(1));
    EXPECT_EQ(nullptr, dsm.GetFirstMatch(idx,1));

    ASSERT_EQ(0,dsm.GetMatchCount(2));
    EXPECT_EQ(nullptr, dsm.GetFirstMatch(idx,2));
}

TEST (DictionaryTermId, Test1) {
    DLP::Dictionaries ds;
    DLP::DictionaryItemFactory dif;
    int16_t score1 = 1;
    int16_t score3 = 3;
    {
        DLP::Dictionary* d = new DLP::Dictionary("A",1,1);
        d->Add(dif.CreateLiteral("aaa", &score3, nullptr, nullptr, nullptr));
        ds.Add(d);
    }
    {
        dif.ResetId();
        DLP::Dictionary* d = new DLP::Dictionary("B",2,1);
        d->Add(dif.CreateLiteral("bbb1", &score1, nullptr, nullptr, nullptr));
        d->Add(dif.CreateLiteral("bbb2", &score3, nullptr, nullptr, nullptr));
        ds.Add(d);
    } 

    DLP::DictionaryScanMatches dsm(&ds);
    // RecordMatch(dictionaryId, itemId, from, to);
    dsm.RecordMatch(1, 1, 5, 10);
    dsm.RecordMatch(1, 1, 15, 20);
    dsm.RecordMatch(2, 1, 7, 12);
    dsm.RecordMatch(2, 2, 23, 27);

    EXPECT_EQ(6, dsm.GetScore(1));
    EXPECT_EQ(4, dsm.GetScore(2));
    EXPECT_EQ(10, dsm.GetTotalScore());

    size_t idx;
    ASSERT_EQ(2,dsm.GetMatchCount(1));
    DLP::Match* m;

    m = dsm.GetFirstMatch(idx,1);
    EXPECT_EQ(5, m->GetFrom());
    EXPECT_EQ(10, m->GetTo());

    m = dsm.GetNextMatch(idx,1);
    EXPECT_EQ(15, m->GetFrom());
    EXPECT_EQ(20, m->GetTo());
    
    EXPECT_EQ(nullptr, dsm.GetNextMatch(idx,1));

    ASSERT_EQ(2,dsm.GetMatchCount(2));

    m = dsm.GetFirstMatch(idx,2);
    EXPECT_EQ(7, m->GetFrom());
    EXPECT_EQ(12, m->GetTo());

    m = dsm.GetNextMatch(idx,2);
    EXPECT_EQ(23, m->GetFrom());
    EXPECT_EQ(27, m->GetTo());
    
    EXPECT_EQ(nullptr, dsm.GetNextMatch(idx,2));

    {
        dif.ResetId();
        DLP::Dictionary* d = new DLP::Dictionary("C",3,1);
        d->Add(dif.CreateLiteral("ccc1", nullptr, nullptr, nullptr, nullptr));
        ds.Add(d);
    }
    dsm.RecordMatch(3, 1, 123, 127);

    EXPECT_EQ(6, dsm.GetScore(1));
    EXPECT_EQ(4, dsm.GetScore(2));
    EXPECT_EQ(1, dsm.GetScore(3));
    EXPECT_EQ(11, dsm.GetTotalScore());

    m = dsm.GetFirstMatch(idx,3);
    EXPECT_EQ(123, m->GetFrom());
    EXPECT_EQ(127, m->GetTo());
}

TEST (DictionaryTermId, TestDupe) {
    DLP::Dictionaries ds;
    DLP::DictionaryItemFactory dif;
    {
        DLP::Dictionary* d = new DLP::Dictionary("A",1,1);
        d->Add(dif.CreateLiteral("aaa", nullptr, nullptr, nullptr, nullptr));
        ds.Add(d);
    }
    {
        dif.ResetId();
        DLP::Dictionary* d = new DLP::Dictionary("B",2,1);
        d->Add(dif.CreateLiteral("bbb1", nullptr, nullptr, nullptr, nullptr));
        ds.Add(d);
    } 

    DLP::DictionaryScanMatches dsm(&ds);
    // RecordMatch(dictionaryId, itemId, from, to);
    dsm.RecordMatch(1, 1, 5, 10);
    dsm.RecordMatch(1, 1, 5, 10);

    dsm.RecordMatch(2, 1, 7, 12);
    dsm.RecordMatch(2, 1, 7, 12);

    EXPECT_EQ(2, dsm.GetScore(1));
    EXPECT_EQ(2, dsm.GetScore(2));
    EXPECT_EQ(0, dsm.GetScore(3)); // No such dictionary
    EXPECT_EQ(4, dsm.GetTotalScore());

    size_t idx;
    ASSERT_EQ(2,dsm.GetMatchCount(1));
    DLP::Match* m;

    m = dsm.GetFirstMatch(idx,1);
    EXPECT_EQ(5, m->GetFrom());
    EXPECT_EQ(10, m->GetTo());

    m = dsm.GetNextMatch(idx,1);
    EXPECT_EQ(5, m->GetFrom());
    EXPECT_EQ(10, m->GetTo());
    
    EXPECT_EQ(nullptr, dsm.GetNextMatch(idx,1));

    ASSERT_EQ(2,dsm.GetMatchCount(2));

    m = dsm.GetFirstMatch(idx,2);
    EXPECT_EQ(7, m->GetFrom());
    EXPECT_EQ(12, m->GetTo());

    m = dsm.GetNextMatch(idx,2);
    EXPECT_EQ(7, m->GetFrom());
    EXPECT_EQ(12, m->GetTo());
    
    EXPECT_EQ(nullptr, dsm.GetNextMatch(idx,2));
}

TEST (DictionaryTermId, Test100) {
    DLP::Dictionaries ds;
    DLP::DictionaryItemFactory dif;
    {
        DLP::Dictionary* d = new DLP::Dictionary("A",1,1);
        d->Add(dif.CreateLiteral("aaa", nullptr, nullptr, nullptr, nullptr));
        ds.Add(d);
    }

    DLP::DictionaryScanMatches dsm(&ds);
    // RecordMatch(dictionaryId, itemId, from, to);
    for( int i=0; i <= 99; ++i) {
        dsm.RecordMatch(1, 1, i, i+5);
    }

    EXPECT_EQ(100, dsm.GetScore(1));
    EXPECT_EQ(100, dsm.GetTotalScore());

    size_t idx;
    ASSERT_EQ(100,dsm.GetMatchCount(1));
    DLP::Match* m;

    m = dsm.GetFirstMatch(idx,1);
    EXPECT_EQ(0, m->GetFrom());
    EXPECT_EQ(5, m->GetTo());

    for( int i=1; i <= 99; ++i) {
        m = dsm.GetNextMatch(idx,1);
        EXPECT_EQ(i, m->GetFrom());
        EXPECT_EQ(i+5, m->GetTo());
    }
    EXPECT_EQ(nullptr, dsm.GetNextMatch(idx,1));
}


