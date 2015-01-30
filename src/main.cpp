#include <iostream>
#include <fstream>
#include <vector>
#include "scanner.h"
#include "weeder.h"

void cleanUp(std::vector<std::vector<Token*> *> tokens)
{
    for (unsigned int i = 0; i < tokens.size(); i++) {
        for(unsigned int j = 0; j < tokens[i]->size(); j++) {
            delete [] tokens[i]->at(j);
        }
        delete [] tokens[i];
    }
}

int main(int argc, char *argv[])
{
    std::ifstream file;
    std::vector<std::vector<Token*> *> tokens;

    Scanner scanner;
    Weeder weeder = Weeder();
    std::vector<Token*> *tokenList;
    std::map<std::string, std::string> symbol_table;
    
    int result;

    for (int i = 1; i <= argc; i++) {
        file.open(argv[i], std::ifstream::in);
        tokenList = new std::vector<Token*>();
        result = scanner.Scan(file, tokenList);
        tokens.push_back(tokenList);
        file.close();
        
        //Error out
        if(result != 0){
            cleanUp(tokens);
            return 42;
        }
    }

    // TODO: Remove Later (:
    std::cout << "Print out tokens to prove it worked :D\n\n";
    for (int i = 0; i < argc; i++) {
        for(unsigned int j = 0; j < tokens[i]->size(); j++) {
            std::cout << tokens[i]->at(j)->getString() << "\n";
        }
    }

    // TODO: Some kind of error checking..
    //       Haven't really discussed how we want
    //       to do this yet.
    ParseTree* tree = buildParseTree(tokens);
    weeder.weedParseTree(tree);
    
    cleanUp(tokens);
    return 0;
}
