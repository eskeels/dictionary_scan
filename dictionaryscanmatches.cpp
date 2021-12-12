#include <iostream>

#include "dictionaryscanmatches.h"

namespace DLP {

DictionaryScanMatches::DictionaryScanMatches(const Dictionaries* dictionaries)
    : dictionaries_(dictionaries) {
}

DictionaryScanMatches::~DictionaryScanMatches() {
}

void DictionaryScanMatches::SetInputBuffer(const char* input, size_t len) {
    input_ = input;
    len_ = len;
}

void DictionaryScanMatches::RecordMatch(uint16_t dictionaryId, uint16_t itemId, unsigned long long from, unsigned long long to) {
   std::cout << dictionaryId << " " << itemId << " " << from << " " << to << std::endl; 
}

void DictionaryScanMatches::RecordMatch(uint16_t dictionaryId, uint16_t itemId, unsigned long long to) {
   std::cout << dictionaryId << " " << itemId << " " << " " << to << std::endl; 
}

}
