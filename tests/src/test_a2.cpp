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
    std::string fileContent;
    std::string buffer;
    std::vector<Token*>* tokenList;
    Weeder weeder = Weeder();
    ParseTree* newParseTree = NULL;
    std::map<std::string, std::vector<Token*> *> tokens;
    std::map<std::string, CompilationUnit*> completeASTs;
    parser = new Parser(tokens);
    BuildCompilationTable compilationBuilder;
    std::map<std::string, CompilationTable*> compilationTables;
    std::map<std::string, std::vector<CompilationTable*> > packagesCompilations;

    std::cout << test_name << ": " << test_description << std::endl;
    std::cout << "---------------------------------------------------------------------------------------" << std::endl;

    // Build STDLib Files First
    std::vector<std::string> files;
    for (unsigned int j = 0; j < STDLIB_NUM_FILES; j++) {
        files.push_back(stdlibPath + "/" + stdlibFiles[j]);
    }

    for (unsigned int j = 0; j < files.size(); j++) {
        file.open(files[j], std::ifstream::in);

        tokenList = new std::vector<Token*>();
        scanner.setFileName(files[j]);
        scanner.Scan(file, tokenList);
        scanner.resetDFAs();
        tokens[files[j]] = tokenList;
        file.close();
    }

    for (unsigned int j = 0; j < files.size(); j++) {
        newParseTree = parser->Parse(files[j]);
        weeder.weedParseTree(newParseTree);
        completeASTs[files[j]] = BuildAst::build(newParseTree);
        delete newParseTree;
    }

    // Build Test File Only
    for(unsigned int i = 0; i < A2_NUM_FILES; i++) {
        Error::resetErrors();

        file.open(directoryPath + "/" + a2TestFiles[i], std::ifstream::in);
        while(getline(file, buffer)) {
            fileContent += buffer + "\n";
        }

        file.clear();
        file.seekg(0, std::ios_base::beg);

        tokenList = new std::vector<Token*>();
        scanner.setFileName(a2TestFiles[i]);
        scanner.Scan(file, tokenList);
        scanner.resetDFAs();
        tokens[a2TestFiles[i]] = tokenList;
        file.close();

        newParseTree = parser->Parse(a2TestFiles[i]);
        weeder.weedParseTree(newParseTree);
        completeASTs[a2TestFiles[i]] = BuildAst::build(newParseTree);
        delete newParseTree;
        compilationTables[a2TestFiles[i]] = compilationBuilder.build(*completeASTs[a2TestFiles[i]], a2TestFiles[i]);
        if (Error::count() == 0) {
            compilationTables[a2TestFiles[i]]->checkForOverlappingLocalScope();
            registerPackages(packagesCompilations, compilationTables[a2TestFiles[i]]);
            setOtherCompilations(compilationTables, packagesCompilations);
        }

        if (Error::count() == 0) {
            TypeLinker(packagesCompilations).typeLinkingResolution();
            HierarchyChecking(packagesCompilations).check();
        }

        if (a2TestFiles[i][1] == 'e') {
            checkTrue("EB-NR-HC: " + a2TestFiles[i], Error::count() != 0,
                      "Check that we fail this file", "\n" + fileContent);
        } else {
            checkTrue("EB-NR-HC: " + a2TestFiles[i], Error::count() == 0,
                      "Check that we pass this file", "\n" + fileContent);
        }

        unregisterPackages(packagesCompilations, compilationTables[a2TestFiles[i]]);
        delete tokens[a2TestFiles[i]];
        delete completeASTs[a2TestFiles[i]];
        delete compilationTables[a2TestFiles[i]];
        tokens.erase(a2TestFiles[i]);
        completeASTs.erase(a2TestFiles[i]);
        compilationTables.erase(a2TestFiles[i]);
    }

    cleanUpTokens(tokens);
    cleanUpASTs(completeASTs);
    delete parser;

    std::cout << "---------------------------------------------------------------------------------------" << std::endl;
}
