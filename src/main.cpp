#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include "error.h"
#include "scanner.h"
#include "weeder.h"
#include "parser.h"
#include "compilationUnit.h"
#include "buildAst.h"
#include "astPrinter.h"

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

void cleanUpASTs(std::map<std::string, CompilationUnit*>& ASTs) {
    std::map<std::string, CompilationUnit*>::iterator it;
    for(it = ASTs.begin(); it != ASTs.end(); it++) {
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
    std::map<std::string, CompilationUnit*> completeASTs;

    // Weeding
    Weeder weeder = Weeder();
    ParseTree* newParseTrees = NULL;
    
    try {
        for (int i = 1; i < argc; i++) {
            filename = argv[i];
            if(filename.find_last_of(".java") == std::string::npos) {
                std::stringstream ss;
                ss << "File '" << filename << "' must end with .java extension.";
                Error(E_DEFAULT, NULL, ss.str());
            } else {
                std::stringstream ss;
                if(filename.length() <= 5) {
                    ss << "File '" << filename << "' cannot fit both a .java extension and a filename.";
                    Error(E_DEFAULT, NULL, ss.str());
                } else {
                    if(filename.compare(filename.length() - 5, 5, ".java") != 0) {
                        ss << "File '" << filename << "' must end with .java extension.";
                        Error(E_DEFAULT, NULL, ss.str());
                    }
                }
            }
            CHECK_ERROR();

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

            newParseTrees = parser.Parse(filename);
            CHECK_ERROR();

            weeder.weedParseTree(newParseTrees);
            CHECK_ERROR();

            completeASTs[filename] = BuildAst::build(newParseTrees);
            AstPrinter::print(*completeASTs[filename]);
            delete newParseTrees;
            newParseTrees = NULL;
        }
    } catch (std::exception &e) {
        Error::print();
        delete newParseTrees;
        rc = 42;
    }
    
    cleanUpTokens(tokens);
    cleanUpASTs(completeASTs);

    exit(rc);
}
