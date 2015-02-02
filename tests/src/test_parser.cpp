#include "a1TestFiles.h"
#include "test_parser.h"
#include "token.h"
#include "parseTree.h"
#include <vector>
#include <map>

Test_Parser::Test_Parser(std::string& directoryPath) : directoryPath(directoryPath) {}

void Test_Parser::init() {
    test_name = "Parser Test";
    test_description = "Testing the parser with Joos 1W files";
}

void Test_Parser::test() {
    std::vector<Token*> * tokens;
    std::ifstream scanFile;
    std::string fileName;
    std::string fileContent;
    std::string buffer;
    ParseTree* parseTree;

    for(unsigned int i = 0; i < A1_NUM_FILES; i++) {
        fileName = a1TestFiles[i];
        tokens = new std::vector<Token*>;
        std::map<std::string, std::vector<Token*> *> parserInput;
        parserInput[fileName] = tokens;
        
        scanFile.open(directoryPath+"/"+fileName, std::ifstream::in);
        while(getline(scanFile, buffer)) {
            fileContent+= buffer + "\n";
        }
        // start from beginning again
        scanFile.clear();
        scanFile.seekg(0, std::ios_base::beg);


        int scanResult = scanner.Scan(scanFile, tokens);
        parser = new Parser(parserInput);
        parseTree = parser->Parse(fileName);

        std::cout << test_name << ": " << test_description << std::endl;
        std::cout << "---------------------------------------------------------------------------------------" << std::endl;

        if(scanResult == SCANNER_OK) {
            if(fileName[1] == 'e') {
                // indicate error file
                checkTrue("Parsing file: " + fileName, parseTree != NULL,
                          "Check if scanner can scan this file", "\n" + fileContent);
            } else {
                checkTrue("Parsing file: " + fileName, parseTree == NULL,
                          "Check if scanner can scan this file", "\n" + fileContent);
            }
        } else {
            checkTrue("Parsing file: " + fileName, true, "Check if parser can parse this file", "\n" + fileContent);
        }
        
        std::cout << "---------------------------------------------------------------------------------------" << std::endl;
        // do resets
        fileContent = "";
        scanFile.close();
        scanner.resetDFAs();
  
        for(unsigned int i = 0; i < tokens->size(); i++) {
            delete (*tokens)[i];
        }

        delete tokens;
        delete parser;
        if(parseTree != NULL) {
            delete parseTree;
        }
    }
}
