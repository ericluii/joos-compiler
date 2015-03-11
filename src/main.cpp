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
#include "buildCompilationTable.h"
#include "typeLinker.h"
#include "hierarchyChecking.h"
#include "packagesManager.h"
#include "ambiguousLinker.h"

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

void cleanUpCompilationTable(std::map<std::string, CompilationTable*>& compilationTables) {
    std::map<std::string, CompilationTable*>::iterator it;
    for(it = compilationTables.begin(); it != compilationTables.end(); it++) {
        delete it->second;
    }
}

void registerPackages(std::map<std::string, std::vector<CompilationTable*> >& packagesCompilations,
                     CompilationTable* table) {
    std::string packageName = table->getPackageName();
    packagesCompilations[packageName].push_back(table);
}

void setOtherCompilations(std::map<std::string, CompilationTable*>& compilationTables,
                          std::map<std::string, std::vector<CompilationTable*> >& packages) {
    std::map<std::string, CompilationTable*>::iterator it;
    for(it = compilationTables.begin(); it != compilationTables.end(); it++) {
        it->second->setCompilationsInPackage(&(packages[it->second->getPackageName()]));
        // check for canonical names here
        it->second->checkForConflictingCanonicalName();
    }
}

void printSymbolTable(SymbolTable* table, int depth = 0) {
    if(table == NULL) return;
    for(int i = 0; i < depth; i++) {
        std::cout << ' ';
    }
    table->printSelf();
    if(table->isClassMethodTable()) {
        printSymbolTable(((ClassMethodTable*)table)->getSymbolTableOfMethod(), depth+1);
    } else if(table->isConstructorTable()) {
        printSymbolTable(((ConstructorTable*)table)->getSymbolTableOfConstructor(), depth+1);
    } else if(table->isNestedBlockTable()) {
        printSymbolTable(((NestedBlockTable*)table)->getSymbolTableOfBlock(), depth+1);
    } else if(table->isForTable()) {
        if(((ForTable*)table)->getForInitTable() != NULL) {
            printSymbolTable(((ForTable*)table)->getForInitTable(), depth+1);
        } else {
            printSymbolTable(((ForTable*)table)->getLoopTable(), depth+1);
        }
    }

    if(!table->isBottomMostEntry()) {
        printSymbolTable(table->getNextTable(), depth);
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
    
    // Symbol table
    BuildCompilationTable compilationBuilder;
    std::map<std::string, CompilationTable*> compilationTables;
    std::map<std::string, std::vector<CompilationTable*> > packagesCompilations;

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
