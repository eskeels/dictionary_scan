#pragma once

namespace DLP
{
class IScanState;

class IScanner {
    public:
        virtual IScanState* CreateScanState() const = 0;
        virtual ~IScanner(){};     
};
}
