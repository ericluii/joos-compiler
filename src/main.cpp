#include <iostream>
#include <fstream>
#include <vector>
#include "scanner.h"

int main(int argc, char *argv[])
{
    std::ifstream file;
    std::vector<std::vector<Token*> *> tokens;
    Scanner scanner;

    for (int i = 1; i <= argc; i++) {
        file.open(argv[i], std::ifstream::in);
        tokens.push_back(scanner.Scan(file));
        file.close();
    }

    std::cout << "Print out tokens to prove it worked :D\n\n";
    for (int i = 0; i < argc; i++) {
        for(unsigned int j = 0; j < tokens[i]->size(); j++) {
            std::cout << tokens[i]->at(j)->getString() << "\n";
        }
    }
}
