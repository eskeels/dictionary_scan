#pragma once

#include <memory>
#include "dictionary.h"

namespace DLP {
class Dictionaries {
    public:
        ~Dictionaries();
        void Add(Dictionary* d);
        // Get dictionary by name
        const Dictionary* GetDictionary(const std::string& name) const;
        // Get dictionary by id
        const Dictionary* GetDictionary(uint16_t id) const;
        // Get the dictionary item by ids
        const IDictionaryItem* GetDictionaryItem(uint16_t dictionaryId, uint16_t itemId) const;
        // Get first dictionary of the ection. Follow up with
        // a call to GetNextDictionary() to iterate through.
        const Dictionary* GetFirstDictionary(size_t& idx) const {
            idx = 0;
            return GetNextDictionary(idx);
        }
        // Get dictionary by index. Set idx to 0 and keep calling
        // until null returned
        const Dictionary* GetNextDictionary(size_t& idx) const;
        // Get number of dictionaries
        size_t GetDictionaryCount() const { return dictionaries_.size(); }
    protected:
        // all the dictionaries
        std::vector<std::unique_ptr<const Dictionary>> dictionaries_;
        // dictionary id to offset in above vector
        // NB: this limits us to 65k dictionaries
        std::unordered_map<uint16_t,uint16_t> dictionariesIndx_;
        // dictionary name to offset
        std::unordered_map<std::string,uint16_t> dictionariesNameIndx_;
};
}
