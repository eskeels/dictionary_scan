#include <iostream>
#include <fstream>
#include <string>

#include "dictionaries.h"
#include "dictionaryitemfactory.h"
#include "json11.hpp"
#include "dictionaryparser.h"


bool processArgs(int argc, char* argv[], std::string& dictionary, std::string& fileToScan) {

    std::string* val = nullptr;
    for (int count = 0; count < argc; ++count) {
        std::string arg(argv[count]);
        if (val != nullptr) {
            *val = arg;
            val = nullptr;
        } else {
            if (arg == "--dictionary" || arg == "-d") {
                val = &dictionary;
            } else if (arg == "--file" || arg == "-f") {
                val = &fileToScan;
            }
        }
    }

    if (dictionary.empty() || fileToScan.empty()) {
        std::cout << "Command line options are: " << std::endl
            << "--dictionary / -d   = Dictionary file to use. JSON format." << std::endl
            << "--file / -f = UTF-8 encoded text file to scan." << std::endl;
        return false;
    }

    return true;
}

int main(int argc, char *argv[]) {

    std::string dictionary;
    std::string fileToScan;

    if (false==processArgs(argc, argv, dictionary, fileToScan)) {
        return 0;
    }

    std::cout << "Scanning " << fileToScan << " with dictionary " << dictionary << std::endl;

    std::ifstream myfile (dictionary);
    std::string dictionaryJson;

    if (myfile.is_open()) {
        std::string line;
        while ( std::getline(myfile,line) ) {
            dictionaryJson.append(line);
            dictionaryJson.append("\n");
        }
        myfile.close();
    }
    else {
        std::cout << "Unable to open file";
        return 0;
    }

    DLP::DictionaryJSONParser djp(dictionaryJson);
    DLP::Dictionaries dictionaries;
    djp.Parse(dictionaries);

    return 0;
}
