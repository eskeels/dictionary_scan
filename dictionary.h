#pragma once

#include <stdint.h>

#include <vector>
#include <unordered_map>

#include "dictionaryitem.h"

namespace DLP
{

class Dictionary {
    public:
        Dictionary(const std::string& name, uint16_t id, uint16_t rev)
            : name_(name),
              id_(id),
              rev_(rev) {}
        void Add(DictionaryItem&& item);

        const DictionaryItem* GetDictionaryItem(uint16_t id) const;
        uint16_t GetId() const { return id_; }
        const std::string& GetName() const { return name_; }
    protected:
        // all the items in this dictionary
        std::vector<DictionaryItem> items_;
        // item id to offset in above vector
        std::unordered_map<uint16_t,uint16_t> itemsIndx_;
        // name of dictionary
        std::string name_;
        // id of dictionary
        uint16_t id_;
        // revision number of dictionary
        uint16_t rev_;
};
}
