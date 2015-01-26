#include <iostream>
#include <fstream>
#include <vector>
#include "scanner.h"

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
    std::vector<Token*> *tokenList;
    Scanner scanner;
    
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

    std::cout << "Print out tokens to prove it worked :D\n\n";
    for (int i = 0; i < argc; i++) {
        for(unsigned int j = 0; j < tokens[i]->size(); j++) {
            std::cout << tokens[i]->at(j)->getString() << "\n";
        }
    }
    
    cleanUp(tokens);
    return 0;
}
