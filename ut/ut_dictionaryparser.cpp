#include <regex>

#include "gtest/gtest.h"

#include "dictionaries.h"
#include "dictionaryitemfactory.h"
#include "json11.hpp"
#include "dictionaryparser.h"

using namespace DLP;

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

