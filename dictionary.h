#pragma once

#include <stdint.h>

#include <memory>
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
        ~Dictionary();
        uint16_t Add(const IDictionaryItem* item);
        uint16_t AddLiteralProximity(std::vector<const IDictionaryItem*> items, uint8_t distance);
        const IDictionaryItem* GetDictionaryItem(uint16_t id) const;
        uint16_t GetId() const { return id_; }
        const std::string& GetName() const { return name_; }
        const IDictionaryItem* GetFirstDictionaryItem(size_t& idx) const {
            idx = 0;
            return GetNextDictionaryItem(idx);
        }
        const IDictionaryItem* GetNextDictionaryItem(size_t& idx) const;
        uint8_t GetProximityDistance(uint16_t proximityId) const {
            auto it = proximityDistance_.find(proximityId);
            if (it != proximityDistance_.end()) {
                return it->second;
            }
            return 0;
        }
        // These are fixed, any value is fine
        uint16_t GetLiteralProximityId() const {
            return 7;
        }
        uint16_t GetRegexProximityId() const {
            return 9;
        }    
    protected:
        // all the items in this dictionary
        std::vector<std::unique_ptr<const IDictionaryItem>> items_;
        // item id to offset in above vector
        std::unordered_map<uint16_t,uint16_t> itemsIndx_;
        // name of dictionary
        std::string name_;
        // id of dictionary
        uint16_t id_;
        // revision number of dictionary
        uint16_t rev_;
        // proximity count. Used for id.
        uint16_t proximityCount_ = 1;
        // proximity map
        // proximity id to offset into items_ vector
        std::unordered_map<uint16_t, std::vector<uint16_t>> proximityItems_;
        // proximity id to distance
        std::unordered_map<uint16_t, uint8_t> proximityDistance_;

};
}
