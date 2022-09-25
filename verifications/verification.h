#pragma once

#include "iverification.h"

namespace DLP {

class Verification : public IVerification {
    public:
        Verification(){}
        virtual ~Verification(){}
        virtual bool Verify(const char* input, size_t inputLen, size_t matchEndPos, std::wstring& out, const char*& matchStartPos) const = 0;
    protected:
        std::wstring GetVerificationTerm(const char* input, 
                                          size_t inputLen, 
                                          size_t matchEndPos,
                                          size_t minTermLen,
                                          size_t maxTermLen,
                                          bool dropSpace,
                                          bool dropPunct,
                                          const char*& matchStartPos) const; 
};

}

