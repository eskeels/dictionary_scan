#include <memory>
#include <regex>

#include "gtest/gtest.h"

#include "hsregexengine.h"
#include "litregexengine.h"
#include "dictionaryitemfactory.h"
#include "dictionaryscanner.h"
#include "dictionaryscanmatches.h"

using namespace DLP;
bool verifyRegex( const std::string& regex, const std::string& txt, bool trigger );

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

enum TermType { LITERAL, REGEX };

void AddDictionary( DLP::Dictionaries& ds,
                    const std::string& name,
                    uint16_t id,
                    const std::vector<std::string> terms,
                    TermType ttype,
                    int16_t score = 20,
                    bool partial = false,
                    bool distiinct = false,
                    bool caseSens = true );

void AddDictionary( DLP::Dictionaries& ds,
                    const std::string& name,
                    uint16_t id,
                    const std::vector<std::string> terms,
                    TermType ttype,
                    int16_t score,
                    bool partial,
                    bool distiinct,
                    bool caseSens) {

    DLP::DictionaryItemFactory ifactory;
    ifactory.SetDefaults(score,
            distiinct,   // distinct
            partial, // partial
            caseSens,    // case sensitive
            10);     // distance

    DLP::Dictionary* d = new Dictionary(name, id, 1);
    for ( auto& t : terms ) {
        if (ttype == LITERAL) {
            d->Add(ifactory.CreateLiteral(t,nullptr,nullptr,nullptr,nullptr));
        } else {
            d->Add(ifactory.CreateRegex(t,nullptr,nullptr,nullptr,nullptr));
        }
    }
    ds.Add(d);
}

std::string getWord();

std::string getWord() {
    std::string tmp; // ("\\W");    
    size_t len = 3+(rand() % 9);
    for (size_t i = 0; i < len; ++i) {
        int randomChar = rand()%(26+26+10);
        if (randomChar < 26)
            tmp.append(1, 'a' + randomChar);
        else if (randomChar < 26+26)
            tmp.append(1, 'A' + randomChar - 26);
        else
            tmp.append(1,'0' + randomChar - 26 - 26);
    }
    return tmp;//+"\\W";
}

class VMatch {
    public:
        VMatch(size_t from, size_t to) : from_(from), to_(to) {}
        VMatch(size_t from, size_t to, uint16_t did) 
            : from_(from),
              to_(to),
              dictionaryId_(did) {}

    size_t from_;
    size_t to_;
    uint16_t dictionaryId_ = 0;
};

bool ScanAndVerify(DLP::Dictionaries& ds, const std::string& txt, std::vector<VMatch> matches);
bool ScanAndVerify(DLP::Dictionaries& ds, const std::string& txt, std::vector<VMatch> matches) {
    DLP::DictionaryScanner dscanner(&ds);
    dscanner.Initialize({});
    std::unique_ptr<IScanState> ss(dscanner.CreateScanState());
    DLP::DictionaryScanMatches dsm(&ds);
    dscanner.Scan(&dsm, &*ss, 0, txt.c_str(), txt.size(), txt.c_str(), txt.size());
    size_t idx;
    size_t matchIdx = 0;
    const Match* m = dsm.GetFirstMatch(idx);
    while(m) {
        std::cout << idx << std::endl;
        if (m->HasFrom() && (m->GetFrom() != matches[matchIdx].from_)) {
            std::cout << "Failed to match from : " << m->GetFrom() << " != " << matches[matchIdx].from_ << std::endl;
            return false;
        }
        if (m->GetTo() != matches[matchIdx].to_) {
            std::cout << "Failed to match to : " << m->GetTo() << " != " << matches[matchIdx].to_ << std::endl;
            return false;
        }
        m = dsm.GetNextMatch(idx);
        matchIdx++;
    }
    return true;
}

TEST (DictionaryOneItem, AllDefault) {
    DLP::Dictionaries ds;
    AddDictionary( ds, "words", 1 , { "sat" }, LITERAL );

    EXPECT_EQ(true, ScanAndVerify(ds, "the cat sat on the mat", {{8,10}}));
}

TEST (DictionaryTwoItems, AllDefault) {
    DLP::Dictionaries ds;
    AddDictionary( ds, "words", 1 , { "sat", "mat" }, LITERAL );

    EXPECT_EQ(true, ScanAndVerify(ds,
                                  "the cat sat on the mat",
                                  {{8,10},  // sat
                                   {19,21}} // mat
                                   ));
}

TEST (DictionaryOneItemTwoTriggers, AllDefault) {
    DLP::Dictionaries ds;
    AddDictionary( ds, "words", 1 , { "the" }, LITERAL );

    EXPECT_EQ(true, ScanAndVerify(ds,
                                  "the cat sat on the mat",
                                  {{0,2},   // the
                                   {15,17}} // the
                                   ));
}

TEST (DictionaryOneItemRegex, AllDefault) {
    DLP::Dictionaries ds;
    AddDictionary( ds, "regexes", 1 , { R"(\d\d)" }, REGEX );

    EXPECT_EQ(true, ScanAndVerify(ds, "the 22 cat sat on the mat 11", {{0,5},{0,27}}));
}

TEST (DictionaryTwoLiteralDictionariesThreeTriggers, AllDefault) {
    DLP::Dictionaries ds;
    AddDictionary( ds, "animals", 1 , { "duck","frog","goat" }, LITERAL );
    AddDictionary( ds, "weapons", 2 , { "axe","knife","hammer" }, LITERAL );

    EXPECT_EQ(true, ScanAndVerify(ds,
                                  "goat axe duck",
                                  {{0,3},   // goat
                                   {5,7},   // axe
                                   {9,12}}  // duck
                                   ));
}
