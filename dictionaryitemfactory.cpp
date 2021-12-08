#include <iostream>

#include "dictionaryitemfactory.h"

namespace DLP {

const std::string WB_START = "(\\W|^)";
const std::string WB_END   = "(\\W|$)";

DictionaryItem DictionaryItemFactory::CreateTerm(const std::string& term, int16_t* score, bool* distinct, bool* partial, bool* caseSensitive)
{
    std::string regex;
    bool makeInsensitive = (caseSensitive != nullptr ? !(*caseSensitive) : defaultCaseSensitive_);
    
    if (makeInsensitive) {
        regex = "(?i)";
    }

    bool makePartial = (partial != nullptr ? *partial : defaultPartial_);

    if (!makePartial) {
        regex.append(WB_START);
    }
    regex.append(term);
    if (!makePartial) {
        regex.append(WB_END);
    }

    DictionaryItem i((makePartial ? ItemType::TermPartial : ItemType::Term),
           (score != nullptr ? *score : defaultScore_),
           (distinct != nullptr ? *distinct : defaultDistinct_),
           GetId(),
           0,
           regex);

    return i;
}
// Phrase
// \bthe[\W\n]{1,10}cat\b
DictionaryItem DictionaryItemFactory::CreatePhrase(int16_t* score, bool* distinct, bool* caseSensitive, uint8_t distance, std::vector<std::string> terms)
{
    std::string regex;
    bool makeInsensitive = (caseSensitive != nullptr ? !(*caseSensitive) : defaultCaseSensitive_);
    
    if (makeInsensitive) {
        regex = "(?i)";
    }

    regex.append(WB_START);

    bool first = true;
    for (auto term : terms) {
        if (!first) {
            regex.append("[\\W\\n]{1," + std::to_string(distance) + "}");
        }
        regex.append(term);
        first = false;
    }

    regex.append(WB_END);
 
    DictionaryItem i(ItemType::Term,
           (score != nullptr ? *score : defaultScore_),
           (distinct != nullptr ? *distinct : defaultDistinct_),
           GetId(),
           0,
           regex);

    return i;
}

// Proximity
// \b(the|cat|mat)[\W\n]{1,10}(the|cat|mat)\b
DictionaryItem DictionaryItemFactory::CreateProximity(int16_t* score, bool* distinct, bool* caseSensitive, uint8_t distance, std::vector<std::string> terms)
{
    std::string regex;
    bool makeInsensitive = (caseSensitive != nullptr ? !(*caseSensitive) : defaultCaseSensitive_);
    
    if (makeInsensitive) {
        regex = "(?i)";
    }

    regex.append(WB_START);
    std::string termList;
    termList.append("(");

    bool first = true;
    for (auto term : terms) {
        if (!first) {
            termList.append("|");
        }
        termList.append(term);
        first = false;
    }
    termList.append(")");

    regex.append(termList);
    regex.append("[\\W\\n]{1," + std::to_string(distance) + "}");
    regex.append(termList);
    regex.append(WB_END);
 
    DictionaryItem i(ItemType::Term,
           (score != nullptr ? *score : defaultScore_),
           (distinct != nullptr ? *distinct : defaultDistinct_),
           GetId(),
           0,
           regex);

    return i;
}

}
