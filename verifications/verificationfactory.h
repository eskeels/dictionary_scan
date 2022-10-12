#pragma once

#include "iverification.h"

namespace DLP {

const uint16_t VERFICATION_UKNI = 1;

class VerificationFactory {
    public:
        static IVerification* CreateVerification(uint16_t id);
};
}

