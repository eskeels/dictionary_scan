#pragma once

#include <stddef.h>
#include <string>

namespace DLP {
class IVerification {
    public:
        IVerification(){}
        virtual ~IVerification() {}
        virtual bool Verify(const char* input, size_t inputLen, size_t matchEndPos, std::wstring& out, const char*& matchStartPos) const = 0;
};

}
