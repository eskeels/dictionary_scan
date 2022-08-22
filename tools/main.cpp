#include <iostream>
#include <fstream>
#include <string>

#include "dictionaries.h"
#include "dictionaryitemfactory.h"
#include "json11.hpp"
#include "dictionaryparser.h"
#include "dictionaryscanner.h"


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

void ReadFileToScan(const std::string& fileName, std::string& txt) {
    std::ifstream myfile (fileName);

    if (myfile.is_open()) {
        std::string line;
        while ( std::getline(myfile,line) ) {
            txt.append(line);
            txt.append("\n");
        }
        myfile.close();
    }
    else {
        std::cout << "Unable to open file";
        return;
    }
}
 
void ReadDictionaries(const std::string& fileName, DLP::Dictionaries& dictionaries) {
    std::ifstream myfile (fileName);
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
        return;
    }

    DLP::DictionaryJSONParser djp(dictionaryJson);
    
    djp.Parse(dictionaries);
}

int main(int argc, char *argv[]) {

    std::string dictionary;
    std::string fileToScan;

    if (false==processArgs(argc, argv, dictionary, fileToScan)) {
        return 0;
    }

    std::cout << "Scanning " << fileToScan << " with dictionary " << dictionary << std::endl;

    DLP::Dictionaries dictionaries;

    ReadDictionaries(dictionary, dictionaries);
    std::string txt;
    ReadFileToScan(fileToScan, txt);

    DLP::DictionaryScanner dscanner(&dictionaries);
    std::string errDesc;
    dscanner.Initialize({},"","",errDesc);
    std::unique_ptr<DLP::IScanState> ss(dscanner.CreateScanState());
    DLP::DictionaryScanMatches dsm(&dictionaries);
    uint8_t context = 99;
    dscanner.Scan(&dsm, &*ss, 0, 0, txt.c_str(), txt.size(), txt.c_str(), txt.size(), context);

    std::vector<std::string> matchedDictionaries;
    dsm.GetMatchedDictionaryNames(matchedDictionaries);
    for (auto name : matchedDictionaries) {
        std::cout << "Matched dictionary " << name << std::endl;
    }

    std::vector<std::string> snippets;

    dsm.CreateMatchSnippets({}, true, 100, 1, snippets, nullptr);
    for( auto& s : snippets) {
        std::cout << "{" << s << "}" << std::endl;
    }

    return 0;
}

