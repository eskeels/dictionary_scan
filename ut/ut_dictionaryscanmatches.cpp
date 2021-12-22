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
        DLP::Dictionary* d = new DLP::Dictionary("B",2,1);
        d->Add(dif.CreateLiteral("bbb1", nullptr, nullptr, nullptr, nullptr));
        d->Add(dif.CreateLiteral("bbb2", nullptr, nullptr, nullptr, nullptr));
        ds.Add(d);
    } 

    DLP::DictionaryScanMatches dsm(&ds);
    dsm.RecordMatch(1, 1, 5, 10);
    dsm.RecordMatch(1, 1, 15, 20);
    dsm.RecordMatch(2, 1, 7, 12);
    dsm.RecordMatch(2, 2, 23, 27);

    size_t idx;
    std::cout << dsm.GetMatchCount(1) << std::endl;
    std::cout << dsm.GetFirstMatch(idx,1)->GetFrom() << std::endl;
    std::cout << dsm.GetFirstMatch(idx,1)->GetTo() << std::endl;

//    std::cout << dsm.GetNextMatch(idx,1)->GetFrom() << std::endl;

//   EXPECT_EQ(3, dti2.GetTermId());
}

