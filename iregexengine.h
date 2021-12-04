#pragma once

namespace DLP {

class Dictionaries;

class IRegexEngine {
    public:
        virtual void Initialise(const Dictionaries* ds) = 0;
        virtual ~IRegexEngine(){};
};

}
