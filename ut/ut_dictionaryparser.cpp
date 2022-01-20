#include <regex>

#include "gtest/gtest.h"

#include "dictionaries.h"
#include "dictionaryitemfactory.h"
#include "json11.hpp"

using namespace DLP;

class DictionaryJSONParser {
    public:
      DictionaryJSONParser(const std::string& input)
        : input_(input)
        {}

     void ProcessFlags(const std::string& flags, bool& distinct, bool& partial, bool& caseSensitive) const {
            for( char c : flags ) {
                switch(c) {
                    case 'c':
                    case 'C':
                        caseSensitive = true;
                        break;
                    case 'd':
                    case 'D':
                        distinct = true;
                        break;
                    case 'p':
                    case 'P':
                        partial = true;
                        break;
                } 
            }
     }

     void ParseLiterals(const json11::Json& literals, DictionaryItemFactory& factory, Dictionary* dictionary) {
         for( auto literal : literals.array_items()) {
             if (literal.is_object()) {
                 std::string val = literal["v"].string_value();
                 if (!val.empty()) {
                     // get 'f' : c = case sensitive (else insensitive), d = distinct (else non distinct), p = partial (else whole)
                     std::string flags = literal["f"].string_value();
                     bool distinct = false;
                     bool partial = false;
                     bool caseSensitive = false;
                     ProcessFlags(flags, distinct, partial, caseSensitive);
                     // get 's' : score
                     int16_t score = literal["s"].int_value();
                     if (score == 0) {
                         score = 10;
                     }
                     IDictionaryItem* item = factory.CreateLiteral(val, &score, &distinct, &partial, &caseSensitive);
                     dictionary->Add(item); 
                 }
             } else {
                 std::string val = literal.string_value();
                 if (!val.empty()) {
                     IDictionaryItem* item = factory.CreateLiteral(val, nullptr, nullptr, nullptr, nullptr);
                     dictionary->Add(item);                              
                 }
                 std::cout << literal.string_value() << std::endl;
             }
         }
     }

     void ParseRegexes(const json11::Json& regexes, DictionaryItemFactory& factory, Dictionary* dictionary) {
         for( auto regex : regexes.array_items()) {
             if (regex.is_object()) {
                 std::string val = regex["v"].string_value();
                 if (!val.empty()) {
                     // get 'f' : c = case sensitive (else insensitive), d = distinct (else non distinct), p = partial (else whole)
                     std::string flags = regex["f"].string_value();
                     bool distinct = false;
                     bool partial = false;
                     bool caseSensitive = false;
                     ProcessFlags(flags, distinct, partial, caseSensitive);
                     // get 's' : score
                     int16_t score = regex["s"].int_value();
                     if (score == 0) {
                         score = 10;
                     }
                     IDictionaryItem* item = factory.CreateLiteral(val, &score, &distinct, &partial, &caseSensitive);
                     dictionary->Add(item); 
                 std::cout << val << std::endl;

                 }
             } else {
                 std::string val = regex.string_value();
                 if (!val.empty()) {
                     IDictionaryItem* item = factory.CreateLiteral(val, nullptr, nullptr, nullptr, nullptr);
                     dictionary->Add(item);                              
                 }
                 std::cout << regex.string_value() << std::endl;
             }
         }
     }
 
     void Parse(Dictionaries& dictionaries) {
          DictionaryItemFactory factory;
          std::string err;
          auto t_obj = json11::Json::parse(input_,err);
          if (!t_obj.is_null() && t_obj.is_object()) {
              auto d_arr = t_obj["dictionaries"];
              if (!d_arr.is_null()) {
                  
                  for (auto& jDic : d_arr.array_items()) {
                      std::cout << "1" << std::endl;
                      std::cout << "Name is " << jDic["name"].string_value() << std::endl;
                      std::string name = jDic["name"].string_value();
                      uint16_t dictionaryId = 0;
                      auto jDictionaryId = jDic["id"];
                      if (!jDictionaryId.is_null() && !jDictionaryId.is_number()) {
                          dictionaryId = jDictionaryId.int_value();
                      }
                      uint16_t revision = 0;
                      auto jRevision = jDic["rev"];
                      if (!jRevision.is_null() && !jRevision.is_number()) {
                          revision = jRevision.int_value();
                      }

                      Dictionary* dictionary = new Dictionary(name, dictionaryId, revision);
                      ParseLiterals(jDic["literals"], factory, dictionary);
                      ParseRegexes(jDic["regexes"], factory, dictionary);

                      dictionaries.Add(dictionary);
                  }
              }
              std::cout<<t_obj.dump()<<" "<<err<<std::endl;
              std::cout<<std::endl;
          }

      }

    protected:
        const std::string& input_;  
};

TEST (DictionaryItemFactoryTest, AllDefault) 
{
    std::string err;
    //creating a json::object as string->string
    std::string test_object = R"(
{
  "dictionaries":[
  {
    "id": 1,
    "name": "dict1",
    "revision": 1,
    "literals": [ "testlit",{"v":"asss","s":10,"f":"cd"},{"v":"bsss","s":10,"f":"cd"}],
    "regexes" : [ {"v":"csss","s":10,"f":"cd"} ]
  }]
}
)";

    DictionaryJSONParser djp(test_object);
    Dictionaries dictionaries;
    djp.Parse(dictionaries);
}

