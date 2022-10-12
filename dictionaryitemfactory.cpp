#include <iostream>

#include "dictionaryitemfactory.h"

namespace DLP {

IDictionaryItem* DictionaryItemFactory::CreateRegex(const std::string& regex, int16_t* score, bool* distinct, bool* partial, bool* caseSensitive, bool proximity)
{
    return new Regex( (score != nullptr ? *score : defaultScore_),
                      (distinct != nullptr ? *distinct : defaultDistinct_),
                      (partial != nullptr ? *partial : defaultPartial_),
                      (caseSensitive != nullptr ? *caseSensitive : defaultCaseSensitive_),
                       GetId(),
                       0, // valgo id
                       regex, proximity );
}

IDictionaryItem* DictionaryItemFactory::CreateLiteral(const std::string& literal, int16_t* score, bool* distinct, bool* partial, bool* caseSensitive, bool proximity)
{
    return new Literal( (score != nullptr ? *score : defaultScore_),
                        (distinct != nullptr ? *distinct : defaultDistinct_),
                        (partial != nullptr ? *partial : defaultPartial_),
                        (caseSensitive != nullptr ? *caseSensitive : defaultCaseSensitive_),
                         GetId(),
                         literal, proximity );
}

std::vector<IDictionaryItem*> DictionaryItemFactory::CreateLiteralProximity(int16_t* score, bool* distinct, bool* partial, bool* caseSensitive, uint8_t /*distance*/, std::vector<std::string> terms)
{
    // create a literal for each term
    // set flag on terms to indicate it is a proximity
    // term has a pointer to proximity structure
    // dictionary can contain 1 literal proximity and 1 regex proximity
    // proximity is ALL literals or ALL regex (can not mix)
    std::vector<IDictionaryItem*> ret;
    for( auto t : terms ) {
        ret.push_back(CreateLiteral(t, score, distinct, partial, caseSensitive, true));
    }
    return ret;
}

/*
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
*/
/*
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
*/
}
