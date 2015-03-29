// AST
#include "classDecl.h"

// Symbol table
#include "classTable.h"

// Code generation related
#include "codeGenerator.h"
#include "startup.h"
#include "vtableManager.h"
#include "compilationTable.h"
#include "vtableLayout.h"
#include "objectLayout.h"

CodeGenerator::CodeGenerator(std::map<std::string, CompilationTable*>& compilations, CompilationTable* firstUnit) :
            compilations(compilations), firstUnit(firstUnit), starter(new Startup(compilations, firstUnit)),
            virtualManager(new VTableManager(compilations)) {}

CodeGenerator::~CodeGenerator() {
    delete starter;
    delete virtualManager;

    std::map<CompilationTable*, ObjectLayout*>::iterator it;
    for(it = layoutOfClasses.begin(); it != layoutOfClasses.end(); it++) {
        delete it->second;
    }
}

void CodeGenerator::initStage() {
    // keep this order
    std::map<std::string, CompilationTable*>::iterator it;
    for(it = compilations.begin(); it != compilations.end(); it++) {
        starter->createTablesForCompilation(it->second);
        virtualManager->createVTableLayoutForCompilation(it->second);
        createObjectLayoutForCompilation(it->second);
    }

    starter->createTablesForArrayType();
    virtualManager->createVTableLayoutForArrays();
    // generate _startup.s
    starter->generateStartupFile(virtualManager->getVTableLayoutOfType(".array"));

    // starter->printInheritanceTable();
    // starter->printInterfaceMethodTable();
}

void CodeGenerator::createObjectLayoutForCompilation(CompilationTable* table) {
    if(table->aTypeWasDefined() && table->isClassSymbolTable()) {
        // a type was defined and it's a class
        if(layoutOfClasses.count(table) == 1) {
            // layout of class for this compilation has been made
            // already, return silently
            return;
        }

        ClassDecl* aClass = ((ClassTable*) table->getSymbolTable())->getClass();
        CompilationTable* superclass = aClass->getSuper()->getSuperClassTable();
        ObjectLayout* superclassLayout = NULL;
        if(superclass != NULL) {
            // create the object layout for the superclass first
            createObjectLayoutForCompilation(superclass);
            superclassLayout = layoutOfClasses[superclass];
        }
        layoutOfClasses[table] = new ObjectLayout(superclassLayout, table);
    }
}
