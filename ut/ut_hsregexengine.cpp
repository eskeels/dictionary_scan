#include <regex>

#include "gtest/gtest.h"

#include "hsregexengine.h"
#include "litregexengine.h"
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

void AddDictionary( DLP::Dictionaries& ds,
                    const std::string& name,
                    uint16_t id,
                    const std::vector<std::string> terms,
                    int16_t score = 20,
                    bool partial = false,
                    bool distiinct = false,
                    bool caseSens = true ) {
    DLP::DictionaryItemFactory ifactory;
    ifactory.SetDefaults(score,
                         false,   // distinct
                         true, // partial
                         true,    // case sensitive
                         10);     // distance

    DLP::Dictionary d(name, id, 1);
    for ( auto& t : terms ) { 
        d.Add(ifactory.CreateTerm(t,nullptr,nullptr,nullptr,nullptr));
    }

    ds.Add(std::move(d));
}

std::string getWord() {
    std::string tmp;    
int len = 1+(rand() % 9);
    for (size_t i = 0; i < len; ++i) {
         int randomChar = rand()%(26+26+10);
         if (randomChar < 26)
             tmp.append(1, 'a' + randomChar);
         else if (randomChar < 26+26)
             tmp.append(1, 'A' + randomChar - 26);
         else
             tmp.append(1,'0' + randomChar - 26 - 26);
    
            
     }
std::cout << tmp << std::endl;
return tmp;
}

TEST (DictionaryItemFactoryTest, AllDefault) {
    DLP::Dictionaries ds;
std::vector<std::string> words;
for (int i = 0; i < 3000; ++i) {
words.push_back(getWord());
}
    AddDictionary( ds, "animals", 1 , words );
/*
    DLP::HSRegexEngine hsre;
    hsre.Register(&ds);
    hsre.Initialize();
    hsre.Serialize();
*/
    DLP::LitRegexEngine chre;
    chre.Register(&ds);
    chre.Initialize();
    chre.Serialize();

}


