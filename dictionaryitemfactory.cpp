#include "dictionaryitemfactory.h"

namespace DLP {

DictionaryItem DictionaryItemFactory::CreateTerm(const std::string& term, int16_t* score, bool* distinct, bool* partial, bool* caseSensitive)
{
    std::string regex;
    bool makeInsensitive = (caseSensitive != nullptr ? !(*caseSensitive) : defaultCaseSensitive_);
    
    if (makeInsensitive) {
        regex = "(?i)";
    }

    bool makePartial = (partial != nullptr ? *partial : defaultPartial_);

    if (!makePartial) {
        regex.append("\\b");
    }
    regex.append(term);
    if (!makePartial) {
        regex.append("\\b");
    }

    DictionaryItem i(ItemType::Term,
           (score != nullptr ? *score : defaultScore_),
           (distinct != nullptr ? *distinct : defaultDistinct_),
           GetId(),
           0,
           regex);

    return i;
}
// Phrase
// \bthe[\W\n]{1,10}cat\b

// Proximity
// \b(the|cat|mat)[\W\n]{1,10}(the|cat|mat)\b
}
