#include "verificationfactory.h"
#include "ukni.h"

namespace DLP {

class AlwaysTrue : public IVerification {
    public:
        AlwaysTrue() {}
        bool Verify(const char* input, 
                    size_t /*inputLen*/, 
                    size_t matchEndPos, 
                    std::wstring& out, 
                    const char*& matchStartPos) const {
            out = L"";
            matchStartPos = input + matchEndPos;
            return true;
        }

};

IVerification* VerificationFactory::CreateVerification(uint16_t id) {
    switch(id) {
        case VERFICATION_UKNI:
        {
            static UKNI ukni;
            return &ukni;
        }
    }

    static AlwaysTrue at;
    return &at;
}

}
