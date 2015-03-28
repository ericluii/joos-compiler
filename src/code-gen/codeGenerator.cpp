#include "codeGenerator.h"
#include "startup.h"
#include "vtableManager.h"
#include "compilationTable.h"
#include "vtableLayout.h"
#include <fstream>

CodeGenerator::CodeGenerator(std::map<std::string, CompilationTable*>& compilations, CompilationTable* firstUnit) :
            compilations(compilations), firstUnit(firstUnit), starter(new Startup(compilations, firstUnit)),
            virtualManager(new VTableManager(compilations)) {}

CodeGenerator::~CodeGenerator() {
    delete starter;
    delete virtualManager;
}

void CodeGenerator::initStage() {
    // keep this order
    std::map<std::string, CompilationTable*>::iterator it;
    for(it = compilations.begin(); it != compilations.end(); it++) {
        starter->createTablesForCompilation(it->second);
        virtualManager->createVTableLayoutForCompilation(it->second);
    }

    starter->createTablesForArrayType();
    virtualManager->createVTableLayoutForArrays();
    // generate !startup.s
    starter->generateStartupFile();

    // starter->printInheritanceTable();
    // starter->printInterfaceMethodTable();
}
