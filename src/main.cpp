#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include "scanner.h"
#include "weeder.h"
#include "parser.h"

void cleanUpTokens(std::map<std::string, std::vector<Token*> *>& tokens)
{
    std::map<std::string, std::vector<Token*> *>::iterator it;
    for (it = tokens.begin(); it != tokens.end(); it++) {
        for(unsigned int i = 0; i < (it->second)->size(); i++) {
            delete (it->second)->at(i);
        }
        delete it->second;
    }
}

void cleanUpParseTrees(std::map<std::string, ParseTree*>& parseTrees) {
    std::map<std::string, ParseTree*>::iterator it;
    for(it = parseTrees.begin(); it != parseTrees.end(); it++) {
        delete it->second;
    }
}

int main(int argc, char *argv[])
{
    std::ifstream file;
    std::map<std::string, std::vector<Token*> *> tokens;

    Scanner scanner;
    Weeder weeder = Weeder();
    std::vector<Token*> *tokenList;
    std::map<std::string, std::string> symbol_table;
    
    int result;

    std::string fileName;
    for (int i = 1; i < argc; i++) {
        fileName = argv[i];
        scanner.setFileName(fileName);
        file.open(fileName, std::ifstream::in);
        if(!file.is_open()){
            std::cerr << "Unable to open file: " << fileName << std::endl;
            cleanUpTokens(tokens);
            exit(42);
        }
        tokenList = new std::vector<Token*>();
        result = scanner.Scan(file, tokenList);
        scanner.resetDFAs();
        tokens[argv[i]] = tokenList;
        file.close();
        
        //Error out
        if(result != SCANNER_OK){
            cleanUpTokens(tokens);
            exit(42);
        }
    }

    // TODO: Remove Later (:
    /*std::cout << "Print out tokens to prove it worked :D\n\n";
    for (int i = 0; i < argc; i++) {
        for(unsigned int j = 0; j < tokens[i]->size(); j++) {
            std::cout << tokens[i]->at(j)->getString() << "\n";
        }
    }*/

    // TODO: Some kind of error checking..
    //       Haven't really discussed how we want
    //       to do this yet.
    
    Parser parser(tokens);
    std::map<std::string, ParseTree*> completeParseTrees;
    for(int i = 1; i < argc; i++) {
        std::string parseFile(argv[i]);
        ParseTree* newParseTrees = parser.Parse(parseFile);
        // error in parsing this file
        if(newParseTrees == NULL) {
            cleanUpTokens(tokens);
            cleanUpParseTrees(completeParseTrees);
            exit(42);
        }
        // Print the parse tree created if needed
        // std::cout << "Parse Tree:\n" << *newParseTrees << std::endl;
        
        completeParseTrees[argv[i]] = newParseTrees;
        weeder.weedParseTree(newParseTrees);
    }
   

    cleanUpTokens(tokens);
    cleanUpParseTrees(completeParseTrees);

    exit(0);
}
