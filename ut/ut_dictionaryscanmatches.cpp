#include "gtest/gtest.h"

#include "dictionaryitemfactory.h"
#include "dictionaryscanmatches.h"

TEST (DictionaryTermId, Test1) {
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
    // RecordMatch(dictionaryId, itemId, from, to);
    dsm.RecordMatch(1, 1, 5, 10);
    dsm.RecordMatch(1, 1, 15, 20);
    dsm.RecordMatch(2, 1, 7, 12);
    dsm.RecordMatch(2, 2, 23, 27);

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
    m = dsm.GetFirstMatch(idx,3);
    EXPECT_EQ(123, m->GetFrom());
    EXPECT_EQ(127, m->GetTo());


}

