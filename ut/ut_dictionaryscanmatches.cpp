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
    EXPECT_EQ(4, dsm.GetMatchCount());

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

TEST (DictionaryTermId, TestWithDistinctRegex) {
    DLP::Dictionaries ds;
    DLP::DictionaryItemFactory dif;
    bool distinct = true;
    {
        DLP::Dictionary* d = new DLP::Dictionary("A",1,1);
        d->Add(dif.CreateRegex("aaa\\d\\d", nullptr, &distinct, nullptr, nullptr));
        ds.Add(d);
    }
    {
        dif.ResetId();
        DLP::Dictionary* d = new DLP::Dictionary("B",2,1);
        d->Add(dif.CreateRegex("bbb1\\d\\d", nullptr, &distinct, nullptr, nullptr));
        ds.Add(d);
    } 

    DLP::DictionaryScanMatches dsm(&ds);
    // RecordMatch(dictionaryId, itemId, from, to);
    dsm.RecordMatch(1, 1, 5, 10);
    dsm.RecordMatch(1, 1, 15, 20);

    dsm.RecordMatch(2, 1, 7, 12);
    dsm.RecordMatch(2, 1, 17, 23);

    EXPECT_EQ(1, dsm.GetScore(1));
    EXPECT_EQ(1, dsm.GetScore(2));
    EXPECT_EQ(2, dsm.GetTotalScore());

    size_t idx;
    ASSERT_EQ(1,dsm.GetMatchCount(1));
    DLP::Match* m;

    m = dsm.GetFirstMatch(idx,1);
    EXPECT_EQ(5, m->GetFrom());
    EXPECT_EQ(10, m->GetTo());

    EXPECT_EQ(nullptr, dsm.GetNextMatch(idx,1));

    ASSERT_EQ(1,dsm.GetMatchCount(2));

    m = dsm.GetFirstMatch(idx,2);
    EXPECT_EQ(7, m->GetFrom());
    EXPECT_EQ(12, m->GetTo());

    EXPECT_EQ(nullptr, dsm.GetNextMatch(idx,2));
}

TEST (DictionaryTermId, TestWithDistinctLiteral) {
    DLP::Dictionaries ds;
    DLP::DictionaryItemFactory dif;
    bool distinct = true;
    {
        DLP::Dictionary* d = new DLP::Dictionary("A",1,1);
        d->Add(dif.CreateLiteral("aaa", nullptr, &distinct, nullptr, nullptr));
        ds.Add(d);
    }
    {
        dif.ResetId();
        DLP::Dictionary* d = new DLP::Dictionary("B",2,1);
        d->Add(dif.CreateLiteral("bbb1", nullptr, &distinct, nullptr, nullptr));
        ds.Add(d);
    } 

    DLP::DictionaryScanMatches dsm(&ds);
    // RecordMatch(dictionaryId, itemId, from, to);
    dsm.RecordMatch(1, 1, 5, 10);
    dsm.RecordMatch(1, 1, 15, 20);

    dsm.RecordMatch(2, 1, 7, 12);
    dsm.RecordMatch(2, 1, 17, 23);

    EXPECT_EQ(1, dsm.GetScore(1));
    EXPECT_EQ(1, dsm.GetScore(2));
    EXPECT_EQ(2, dsm.GetTotalScore());

    size_t idx;
    ASSERT_EQ(1,dsm.GetMatchCount(1));
    DLP::Match* m;

    m = dsm.GetFirstMatch(idx,1);
    EXPECT_EQ(5, m->GetFrom());
    EXPECT_EQ(10, m->GetTo());

    EXPECT_EQ(nullptr, dsm.GetNextMatch(idx,1));

    ASSERT_EQ(1,dsm.GetMatchCount(2));

    m = dsm.GetFirstMatch(idx,2);
    EXPECT_EQ(7, m->GetFrom());
    EXPECT_EQ(12, m->GetTo());

    EXPECT_EQ(nullptr, dsm.GetNextMatch(idx,2));
}

TEST (DictionaryTermId, Test100WithDistinct) {
    DLP::Dictionaries ds;
    DLP::DictionaryItemFactory dif;
    bool distinct = true;
    {
        DLP::Dictionary* d = new DLP::Dictionary("A",1,1);
        d->Add(dif.CreateLiteral("aaa", nullptr, &distinct, nullptr, nullptr));
        ds.Add(d);
    }

    DLP::DictionaryScanMatches dsm(&ds);
    // RecordMatch(dictionaryId, itemId, from, to);
    for( int i=0; i <= 99; ++i) {
        dsm.RecordMatch(1, 1, i, i+5);
    }

    EXPECT_EQ(1, dsm.GetScore(1));
    EXPECT_EQ(1, dsm.GetTotalScore());

    size_t idx;
    ASSERT_EQ(1,dsm.GetMatchCount(1));
    DLP::Match* m;

    m = dsm.GetFirstMatch(idx,1);
    EXPECT_EQ(0, m->GetFrom());
    EXPECT_EQ(5, m->GetTo());

    EXPECT_EQ(nullptr, dsm.GetNextMatch(idx,1));
}

TEST (DictionaryTermId, TestSnippet) {
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

    std::string s("the aaa the bbb1 and bbb2");
    DLP::DictionaryScanMatches dsm(&ds);
    dsm.SetInputBuffer(s.c_str(), s.size());
    // RecordMatch(dictionaryId, itemId, from, to);
    dsm.RecordMatch(1, 1, 4, 7);
    dsm.RecordMatch(2, 2, 12, 16);
    dsm.RecordMatch(2, 2, 21, 25);

    { 
    std::set<uint16_t> dictionaryIds;
    std::vector<std::string> snippets;
    dsm.CreateMatchSnippets(dictionaryIds, true /* all */, 5 /* count */, 0 /* affix */, snippets);

    EXPECT_EQ("bbb1", snippets[0]);
    EXPECT_EQ("bbb2", snippets[1]);
    EXPECT_EQ("aaa", snippets[2]);
    }

    { 
    std::set<uint16_t> dictionaryIds;
    std::vector<std::string> snippets;
    dsm.CreateMatchSnippets(dictionaryIds, true /* all */, 5 /* count */, 1 /* affix */, snippets);

    EXPECT_EQ(" bbb1 ", snippets[0]);
    EXPECT_EQ(" bbb2", snippets[1]);
    EXPECT_EQ(" aaa ", snippets[2]);
    }

    { 
    std::set<uint16_t> dictionaryIds;
    std::vector<std::string> snippets;
    dsm.CreateMatchSnippets(dictionaryIds, true /* all */, 5 /* count */, 3 /* affix */, snippets);

    EXPECT_EQ("he bbb1 an", snippets[0]);
    EXPECT_EQ("nd bbb2", snippets[1]);
    EXPECT_EQ("he aaa th", snippets[2]);
    }
}

TEST (DictionaryTermId, TestSnippetFilterByDictionary) {
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

    std::string s("the aaa the bbb1 and bbb2");
    DLP::DictionaryScanMatches dsm(&ds);
    dsm.SetInputBuffer(s.c_str(), s.size());
    // RecordMatch(dictionaryId, itemId, from, to);
    dsm.RecordMatch(1, 1, 4, 7);
    dsm.RecordMatch(2, 2, 12, 16);
    dsm.RecordMatch(2, 2, 21, 25);

    { 
    std::set<uint16_t> dictionaryIds({1,2});
    std::vector<std::string> snippets;
    dsm.CreateMatchSnippets(dictionaryIds, false /* all */, 5 /* count */, 0 /* affix */, snippets);
    EXPECT_EQ("bbb1", snippets[0]);
    EXPECT_EQ("bbb2", snippets[1]);
    EXPECT_EQ("aaa", snippets[2]);
    }
    { 
    std::set<uint16_t> dictionaryIds({1});
    std::vector<std::string> snippets;
    dsm.CreateMatchSnippets(dictionaryIds, false /* all */, 5 /* count */, 0 /* affix */, snippets);
    EXPECT_EQ("aaa", snippets[0]);
    }
    { 
    std::set<uint16_t> dictionaryIds({2});
    std::vector<std::string> snippets;
    dsm.CreateMatchSnippets(dictionaryIds, false /* all */, 5 /* count */, 0 /* affix */, snippets);
    EXPECT_EQ("bbb1", snippets[0]);
    EXPECT_EQ("bbb2", snippets[1]);
    }
    { 
    std::set<uint16_t> dictionaryIds({3});
    std::vector<std::string> snippets;
    dsm.CreateMatchSnippets(dictionaryIds, false /* all */, 5 /* count */, 0 /* affix */, snippets);
    ASSERT_EQ(0, snippets.size());
    }
}

TEST (DictionaryTermId, TestSnippetFilterByContext) {
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

    std::string s("the aaa the bbb1 and bbb2");
    DLP::DictionaryScanMatches dsm(&ds);
    dsm.SetInputBuffer(s.c_str(), s.size());
    // RecordMatch(dictionaryId, itemId, from, to);
    dsm.SetContext(1);
    dsm.RecordMatch(1, 1, 4, 7);
    dsm.RecordMatch(2, 2, 12, 16);
    
    dsm.SetContext(3);
    dsm.RecordMatch(2, 2, 21, 25);

    { 
    std::set<uint16_t> dictionaryIds({1,2});
    std::vector<std::string> snippets;
    dsm.CreateMatchSnippets(dictionaryIds, false /* all */, 5 /* count */, 0 /* affix */, snippets, nullptr);
    EXPECT_EQ("bbb1", snippets[0]);
    EXPECT_EQ("bbb2", snippets[1]);
    EXPECT_EQ("aaa", snippets[2]);
    }
    { 
    std::set<uint16_t> dictionaryIds({1,2});
    std::vector<std::string> snippets;
    uint8_t context = 1;
    dsm.CreateMatchSnippets(dictionaryIds, false /* all */, 5 /* count */, 0 /* affix */, snippets, &context);
    EXPECT_EQ("bbb1", snippets[0]);
    EXPECT_EQ("aaa", snippets[1]);
    }
    { 
    std::set<uint16_t> dictionaryIds({1,2});
    std::vector<std::string> snippets;
    uint8_t context = 3;
    dsm.CreateMatchSnippets(dictionaryIds, false /* all */, 5 /* count */, 0 /* affix */, snippets, &context);
    EXPECT_EQ("bbb2", snippets[0]);
    }
    { 
    std::set<uint16_t> dictionaryIds({1,2});
    std::vector<std::string> snippets;
    uint8_t context = 5;
    dsm.CreateMatchSnippets(dictionaryIds, false /* all */, 5 /* count */, 0 /* affix */, snippets, &context);
    EXPECT_EQ(0, snippets.size());
    }
}
