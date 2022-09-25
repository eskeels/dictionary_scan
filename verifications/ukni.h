#pragma once

#include "verification.h"

namespace DLP {

class UKNI : public Verification {
    public:
        UKNI(){}
        virtual bool Verify(const char* input, size_t inputLen, size_t matchEndPos, std::wstring& out, const char*& matchStartPos) const;
};

}

