#include "a1TestFiles.h"
#include "test_scanner.h"
#include "token.h"
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

void Test_Scanner::init() {
    test_name = "Scanner Test";
    test_description = "Testing the scanner Joos 1W files";
}

void Test_Scanner::test() {
    std::stringstream ss;
    std::vector<Token*> * tokens;
    std::ifstream scanFile;
    std::string fileName;
    std::string fileContent;
    std::string buffer;

    std::cout << test_name << ": " << test_description << std::endl;
    std::cout << "---------------------------------------------------------------------------------------" << std::endl;

    for(unsigned int i = 0; i < a1TestFiles.size(); i++) {
        fileName = a1TestFiles[i][0];
        scanner.setFileName(fileName);
        scanFile.open(directoryPath+"/"+fileName, std::ifstream::in);
        tokens = new std::vector<Token*>;

        while(getline(scanFile, buffer)) {
            fileContent+= buffer + "\n";
        }
        // start from beginning again
        scanFile.clear();
        scanFile.seekg(0, std::ios_base::beg);

        int scanResult = scanner.Scan(scanFile, tokens);

        if(fileName[1] == 'e') {
            ss.str(std::string());
            ss << scanResult;
            checkTrue("Scanning file: " + fileName, true,
                      "Ambiguous result, logging.", ss.str());
        } else {
            checkTrue("Scanning file: " + fileName, scanResult == SCANNER_OK,
                      "Check if scanner can scan this file", "\n" + fileContent);
        }
        
        // do resets
        fileContent = "";
        scanFile.close();
        scanner.resetDFAs();
        
        for(unsigned int i = 0; i < tokens->size(); i++) {
            delete (*tokens)[i];
        }

        delete tokens;
    }

        std::cout << "---------------------------------------------------------------------------------------" << std::endl;
}

Test_Scanner::Test_Scanner(std::string& directoryPath) : directoryPath(directoryPath) {}
