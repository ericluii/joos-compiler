#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include "error.h"
#include "scanner.h"
#include "weeder.h"
#include "parser.h"
#include "AST/ast.h"

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
    int rc = 0;
    std::string filename;
    std::ifstream file;

    // Scanning
    Scanner scanner;
    std::map<std::string, std::vector<Token*> *> tokens;
    std::vector<Token*> *tokenList;

    // Parsing
    Parser parser(tokens);
    std::map<std::string, ParseTree*> completeParseTrees;

    // Weeding
    Weeder weeder = Weeder();

    try {

        for (int i = 1; i < argc; i++) {
            filename = argv[i];

            file.open(filename, std::ifstream::in);
            if(!file.is_open()){
                std::stringstream ss;
                ss << "Unable to open file: " << filename << ".";
                Error(E_DEFAULT, NULL, ss.str()); 
                CHECK_ERROR();
            }

            tokenList = new std::vector<Token*>();
            scanner.setFileName(filename);
            scanner.Scan(file, tokenList);
            scanner.resetDFAs();
            tokens[filename] = tokenList;
            file.close();

            CHECK_ERROR();
        }

        for(int i = 1; i < argc; i++) {
            filename = argv[i];

            ParseTree* newParseTrees = parser.Parse(filename);
            CHECK_ERROR();

            weeder.weedParseTree(newParseTrees);
            CHECK_ERROR();

            completeParseTrees[filename] = newParseTrees;
        }
    } catch (std::exception &e) {
        Error::print();
        rc = 42;
    }
    
    std::cout << "starting\n";
    CompilationUnit *unit = ast::makeCompilationUnit(completeParseTrees[argv[1]]);
    
    cleanUpTokens(tokens);
    cleanUpParseTrees(completeParseTrees);

    exit(rc);
}
