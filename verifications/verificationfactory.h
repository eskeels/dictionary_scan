#pragma once

#include "iverification.h"

namespace DLP {

class VerificationFactory {
    public:
        static IVerification* CreateVerification(uint16_t id);
};
}

