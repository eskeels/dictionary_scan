#include "gtest/gtest.h"

#include "dictionarytermid.h"

TEST (DictionaryTermId, Test) {
    DLP::DictionaryTermId dti(1,3);
    uint32_t id = dti.GetId();
    std::cout << id << std::endl; 
    DLP::DictionaryTermId dti2(id);
    std::cout << dti2.GetDictionaryId() << " " << dti2.GetTermId() << std::endl;
}
