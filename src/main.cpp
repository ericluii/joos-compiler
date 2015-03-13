#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include "error.h"
#include "test_main_common.h"
#include "scanner.h"
#include "weeder.h"
#include "parser.h"
#include "compilationUnit.h"
#include "buildAst.h"
#include "astPrinter.h"
#include "buildCompilationTable.h"
#include "typeLinker.h"
#include "hierarchyChecking.h"
#include "packagesManager.h"
#include "ambiguousLinker.h"
#include "typeChecker.h"

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
    
    // Symbol table
    BuildCompilationTable compilationBuilder;
    std::map<std::string, CompilationTable*> compilationTables;
    std::map<std::string, std::vector<CompilationTable*> > packagesCompilations;

    try {
        for (int i = 1; i < argc; i++) {
            filename = argv[i];
            if(filename.find(".java") == std::string::npos) {
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
            delete newParseTrees;
            newParseTrees = NULL;

            // AstPrinter::print(*completeASTs[filename]);
            compilationTables[filename] = compilationBuilder.build(*completeASTs[filename], filename);
            CHECK_ERROR();

            compilationTables[filename]->checkForOverlappingLocalScope();
            CHECK_ERROR();
            /* if(compilationTables[filename]->getSymbolTable() != NULL) {
                printSymbolTable(compilationTables[filename]->getSymbolTable());
            }*/
            
            registerPackages(packagesCompilations, compilationTables[filename]);
        }

        setOtherCompilations(compilationTables, packagesCompilations);
        CHECK_ERROR();

        TypeLinker(packagesCompilations).typeLinkingResolution();
        CHECK_ERROR();

        HierarchyChecking(packagesCompilations).check();
        CHECK_ERROR();

        PackagesManager pkgManager(packagesCompilations);
        AmbiguousLinker(pkgManager, packagesCompilations).performLinking();
        CHECK_ERROR();
        TypeChecking(pkgManager, packagesCompilations).check();
        CHECK_ERROR();

    } catch (std::exception &e) {
        Error::print();
        delete newParseTrees;
        rc = 42;
    }
   
    cleanUpTokens(tokens);
    cleanUpASTs(completeASTs);
    cleanUpCompilationTable(compilationTables);

    exit(rc);

}
