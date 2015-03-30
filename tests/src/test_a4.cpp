#include "a4TestFiles.h"
#include "test_main_common.h"
#include "joos_stdlib.h"
#include "test_a4.h"
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
#include "packagesManager.h"
#include "ambiguousLinker.h"
#include "typeChecker.h"
#include "reachable.h"

Test_A4::Test_A4(std::string& stdlibPath, std::string& directoryPath) :
    directoryPath(directoryPath),
    stdlibPath(stdlibPath) {}

void Test_A4::init() {
    test_name = "Static Analysis.";
    test_description = "This tests encapsualtes all A4 required tests.";
}

void Test_A4::test() {
    std::ifstream file;
    ParseTree* newParseTree = NULL;
    std::string fileContent = "File too long.";

    std::cout << test_name << ": " << test_description << std::endl;
    std::cout << "---------------------------------------------------------------------------------------" << std::endl;

    // Build Test File Only
    for(unsigned int i = 0; i < a4TestFiles.size(); i++) {
        Error::resetErrors();

        std::map<std::string, std::vector<Token*> *> tokens;
        std::map<std::string, CompilationUnit*> completeASTs;
        Weeder weeder = Weeder();
        std::vector<Token*>* tokenList;
        parser = new Parser(tokens);
        std::vector<std::string> files;
        for (unsigned int j = 0; j < stdlibFilesA4.size(); j++) {
            files.push_back(stdlibPath + "/" + stdlibFilesA4[j]);
        }

        for (unsigned int j = 0; j < a4TestFiles[i].size(); j++) {
            files.push_back(directoryPath + "/" + a4TestFiles[i][j]); 
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

        if (Error::count() == 0) {
            PackagesManager pkgManager(packagesCompilations);
            AmbiguousLinker(pkgManager, packagesCompilations).performLinking();
            if (Error::count() == 0) {
                TypeChecking(pkgManager, packagesCompilations).check();

                if (Error::count() == 0) {
                    Reachable(packagesCompilations).checkReachability();
                }
            }
        }

        if (a4TestFiles[i][0][1] == 'e') {
            checkTrue("A4: " + a4TestFiles[i][0], Error::count() != 0,
                      "Check that we fail this file", fileContent);
        } else {
            checkTrue("A4: " + a4TestFiles[i][0], Error::count() == 0,
                      "Check that we pass this file", fileContent);
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
