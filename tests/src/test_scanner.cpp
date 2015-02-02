#include "a1TestFiles.h"
#include "test_scanner.h"
#include "token.h"
#include <iostream>
#include <vector>
#include <fstream>

void Test_Scanner::init() {
    test_name = "Scanner Test";
    test_description = "Testing the scanner Joos 1W files";
}

void Test_Scanner::test() {
    std::vector<Token*> * tokens;
    std::ifstream scanFile;
    std::string fileName;
    std::string fileContent;
    std::string buffer;

    for(unsigned int i = 0; i < A1_NUM_FILES; i++) {
        fileName = a1TestFiles[i];
        scanner.setFileName(fileName);
        scanFile.open(directoryPath+"/"+fileName, std::ifstream::in);
        tokens = new std::vector<Token*>;
        std::cout << test_name << ": " << test_description << std::endl;
        std::cout << "---------------------------------------------------------------------------------------" << std::endl;

        while(getline(scanFile, buffer)) {
            fileContent+= buffer + "\n";
        }
        // start from beginning again
        scanFile.clear();
        scanFile.seekg(0, std::ios_base::beg);

        int scanResult = scanner.Scan(scanFile, tokens);

        checkTrue("Scanning file: " + fileName, scanResult == SCANNER_ABORT || scanResult == SCANNER_OK,
                 "Check if scanner can scan this file", "\n" + fileContent);
        
        std::cout << "---------------------------------------------------------------------------------------" << std::endl;
        // do resets
        fileContent = "";
        scanFile.close();
        scanner.resetDFAs();
        
        for(unsigned int i = 0; i < tokens->size(); i++) {
            delete (*tokens)[i];
        }

        delete tokens;
    }
}

Test_Scanner::Test_Scanner(std::string& directoryPath) : directoryPath(directoryPath) {}
