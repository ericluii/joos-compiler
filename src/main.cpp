#include <iostream>
#include <fstream>
#include <vector>
#include "scanner.h"
#include "weeder.h"

int main(int argc, char *argv[])
{
    std::ifstream file;
    std::vector<std::vector<Token*> *> tokens;

    Scanner scanner;
    Weeder weeder = Weeder();

    for (int i = 1; i <= argc; i++) {
        file.open(argv[i], std::ifstream::in);
        tokens.push_back(scanner.Scan(file));
        file.close();
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
}
