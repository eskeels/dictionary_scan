#include <memory>
#include <regex>

#include "gtest/gtest.h"

#include "hsregexengine.h"
#include "litregexengine.h"
#include "dictionaryitemfactory.h"
#include "dictionaryscanner.h"
#include "dictionaryscanmatches.h"

#include "dictionaryscannerhelper.h"

using namespace DLP;
/*
TEST (DictionaryEmpty, AllDefault) {
    DLP::Dictionaries ds;

    DLP::DictionaryScanMatches dsm = Scan(ds, "shouldnt get any triggers", 1);
    EXPECT_EQ(size_t(0), dsm.GetMatchCount());
    EXPECT_EQ(int64_t(0), dsm.GetTotalScore());
}
*/

TEST (DictionaryOneItem, AllDefault) {

    DLP::Dictionaries ds;

    AddDictionary( ds, "animals", 1 , { "mat", "sat" }, TermType::LITERAL );

    std::string txt("the cat sat on the mat");
    std::string errDesc;

    {
    DLP::DictionaryScanner dscanner(&ds);
    dscanner.Initialize({},"", "", errDesc);
    dscanner.Serialize("literal-db.bin", "regex-db.bin", errDesc);
    }

    DLP::DictionaryScanner dscanner(&ds);
    dscanner.Initialize({},"literal-db.bin", "regex-db.bin", errDesc);
  
    std::unique_ptr<IScanState> ss(dscanner.CreateScanState());
    DLP::DictionaryScanMatches dsm(&ds);
    dscanner.Scan(&dsm, &*ss, 0, 0, txt.c_str(), txt.size(), txt.c_str(), txt.size(), 0);

    size_t idx = 0;
    Match* pm = dsm.GetFirstMatch(idx);
    while (pm) {
        std::cout << "Matched at " << pm->GetFrom() << " " << pm->GetItemId() << " " << std::endl;
        pm = dsm.GetNextMatch(idx);
    }

}


