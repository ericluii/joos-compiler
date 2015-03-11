#include "a2TestFiles.h"
#include "joos_stdlib.h"
#include "test_a2.h"
#include "error.h"
#include "token.h"
#include "parseTree.h"
#include "weeder.h"
#include "buildAst.h"
#include "buildCompilationTable.h"
#include "typeLinker.h"
#include "hierarchyChecking.h"
#include <fstream>
#include <vector>
#include <map>
#include <iostream>

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

void setOtherCompilations(std::map<std::string, CompilationTable*>& compilationTables,
                          std::map<std::string, std::vector<CompilationTable*> >& packages) {
    std::map<std::string, CompilationTable*>::iterator it;
    for(it = compilationTables.begin(); it != compilationTables.end(); it++) {
        it->second->setCompilationsInPackage(&(packages[it->second->getPackageName()]));
        // check for canonical names here
        it->second->checkForConflictingCanonicalName();
    }
}

void registerPackages(std::map<std::string, std::vector<CompilationTable*> >& packagesCompilations,
                             CompilationTable* table) {
    std::string packageName = table->getPackageName();
    packagesCompilations[packageName].push_back(table);
}

void unregisterPackages(std::map<std::string, std::vector<CompilationTable*> >& packagesCompilations,
                        CompilationTable* table) {
    std::string packageName = table->getPackageName();
    packagesCompilations.erase(packageName);
}

void cleanUpASTs(std::map<std::string, CompilationUnit*>& ASTs) {
    std::map<std::string, CompilationUnit*>::iterator it;
    for(it = ASTs.begin(); it != ASTs.end(); it++) {
        delete it->second;
    }
}

Test_A2::Test_A2(std::string& stdlibPath, std::string& directoryPath) :
    directoryPath(directoryPath),
    stdlibPath(stdlibPath) {}

void Test_A2::init() {
    test_name = "Test Environment Building, Type Linking, and Hierarchy Checking";
    test_description = "This tests encapsualtes all A2 required tests.";
}

void Test_A2::test() {
    std::ifstream file;
    ParseTree* newParseTree = NULL;
    std::string fileContent = "File too long.";

    std::cout << test_name << ": " << test_description << std::endl;
    std::cout << "---------------------------------------------------------------------------------------" << std::endl;



    // Build Test File Only
    for(unsigned int i = 0; i < a2TestFiles.size(); i++) {
        Error::resetErrors();


        std::map<std::string, std::vector<Token*> *> tokens;
        std::map<std::string, CompilationUnit*> completeASTs;
        Weeder weeder = Weeder();
        std::vector<Token*>* tokenList;
        parser = new Parser(tokens);
        std::vector<std::string> files;
        for (unsigned int j = 0; j < STDLIB_NUM_FILES; j++) {
            files.push_back(stdlibPath + "/" + stdlibFiles[j]);
        }
        for (unsigned int j = 0; j < a2TestFiles[i].size(); j++) {
            files.push_back(directoryPath + "/" + a2TestFiles[i][j]); 
        }

        for (unsigned int j = 0; j < files.size(); j++) {
            file.open(files[j]);
            tokenList = new std::vector<Token*>();
            scanner.setFileName(files[j]);
            scanner.Scan(file, tokenList);
            scanner.resetDFAs();
            tokens[files[j]] = tokenList;
            file.close();
        }

        BuildCompilationTable compilationBuilder;
        std::map<std::string, CompilationTable*> compilationTables;
        std::map<std::string, std::vector<CompilationTable*> > packagesCompilations;

        for (unsigned int j = 0; j < files.size(); j++) {
            newParseTree = parser->Parse(files[j]);
            if (newParseTree == NULL) { break; }
            weeder.weedParseTree(newParseTree);
            completeASTs[files[j]] = BuildAst::build(newParseTree);
            delete newParseTree;
            compilationTables[files[j]] = compilationBuilder.build(*completeASTs[files[j]], files[j]);
            compilationTables[files[j]]->checkForOverlappingLocalScope();
            registerPackages(packagesCompilations, compilationTables[files[j]]);
        }

        setOtherCompilations(compilationTables, packagesCompilations);

        if (Error::count() == 0) {
            TypeLinker(packagesCompilations).typeLinkingResolution();
        }

        if (Error::count() == 0) {
            HierarchyChecking(packagesCompilations).check();
        }


        if (a2TestFiles[i][0][1] == 'e') {
            checkTrue("EB-NR-HC: " + a2TestFiles[i][0], Error::count() != 0,
                      "Check that we fail this file", "\n" + fileContent);
        } else {
            checkTrue("EB-NR-HC: " + a2TestFiles[i][0], Error::count() == 0,
                      "Check that we pass this file", "\n" + fileContent);
        }

        for (unsigned int j = 0; j < files.size(); j++) {
            delete compilationTables[files[j]];
        }

        cleanUpTokens(tokens);
        cleanUpASTs(completeASTs);

        delete parser;
    }

    std::cout << "---------------------------------------------------------------------------------------" << std::endl;
}
