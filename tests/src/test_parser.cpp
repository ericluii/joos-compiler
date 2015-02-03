#include "a1TestFiles.h"
#include "test_parser.h"
#include "token.h"
#include "parseTree.h"
#include "weeder.h"
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
    Weeder weeder;

    std::cout << test_name << ": " << test_description << std::endl;
    std::cout << "---------------------------------------------------------------------------------------" << std::endl;

    for(unsigned int i = 0; i < A1_NUM_FILES; i++) {
        fileName = a1TestFiles[i];
        scanner.setFileName(fileName);
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

        if(scanResult == SCANNER_OK) {
            parseTree = parser->Parse(fileName);
            if(fileName[1] == 'e') {
                if (parseTree) {
                    checkTrue("Weeding file: " + fileName, weeder.weedParseTree(parseTree) != 0,
                              "Ensure weeder fails this file", "\n" + fileContent);
                } else {
                    // indicate error file
                    checkTrue("Parsing file: " + fileName, parseTree == NULL,
                              "Ensure parser can't parse this file", "\n" + fileContent);
                }
            } else {
                if (parseTree) {
                    checkTrue("Weeding file: " + fileName, weeder.weedParseTree(parseTree) == 0,
                              "Check weeder passes this file", "\n" + fileContent);
                } else {
                    checkTrue("Parsing file: " + fileName, parseTree != NULL,
                              "Check if parser can parse this file", "\n" + fileContent);
                }
            }

            for(unsigned int i = 0; i < tokens->size(); i++) {
                delete (*tokens)[i];
            }

            delete tokens;
            delete parser;
            if(parseTree != NULL) {
                delete parseTree;
            }
        } else {
            // Unscannable file is being tested - quietely skip
        }
        
        // do resets
        fileContent = "";
        scanFile.close();
        scanner.resetDFAs();
    }

    std::cout << "---------------------------------------------------------------------------------------" << std::endl;
}
