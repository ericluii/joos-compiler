#include <cassert>

#include "implementedInterfaceMethodTable.h"
#include "implementedInterfaceMethodTableManager.h"

// Symbol table
#include "compilationTable.h"
#include "classTable.h"
#include "interfaceMethodTable.h"

// AST
#include "classDecl.h"
#include "classMethod.h"
#include "interfaceMethod.h"

// Label
#include "labelManager.h"

#include<iostream>

ImplInterfaceMethodTableManager::ImplInterfaceMethodTableManager(std::map<std::string, CompilationTable*>& compilations) :
        methodCounter(0), numMethods(0) {
    initializeMapping(compilations);
    std::map<std::string, CompilationTable*>::iterator it;
    for(it = compilations.begin(); it != compilations.end(); it++) {
        if(it->second->aTypeWasDefined() && it->second->isClassSymbolTable()) {
            // for all class types
            generateTableForClass(it->second);
        }
    }
    generateTableForArray();
}

ImplInterfaceMethodTableManager::~ImplInterfaceMethodTableManager() {
    std::map<std::string, ImplInterfaceMethodTable*>::iterator it;
    for(it = tables.begin(); it != tables.end(); it++) {
        delete it->second;
    }
}

void ImplInterfaceMethodTableManager::initializeMapping(std::map<std::string, CompilationTable*>& compilations) {
    std::map<std::string, CompilationTable*>::iterator it;
    for(it = compilations.begin(); it != compilations.end(); it++) {
        if(it->second->aTypeWasDefined() && !it->second->isClassSymbolTable()) {
            // for all interface types
            std::map<std::string, InterfaceMethodTable*>& methods = it->second->getDefinedInterfaceMethods();
            std::map<std::string, InterfaceMethodTable*>::iterator it2;
            for(it2 = methods.begin(); it2 != methods.end(); it2++) {
                // register interface methods
                std::string methodSignature = it2->second->getInterfaceMethod()->methodSignatureAsString();
                if(methodMapping.count(methodSignature) == 0) {
                    // method with particular signature has not been
                    // registered yet
                    methodMapping[methodSignature] = methodCounter++;
                    numMethods++;
                }
            }

        } else if(it->second->aTypeWasDefined()) {
            // then this must be a class
            if(it->second->getCanonicalName() == "java.lang.Object") {
                // it's java.lang.Object, then register it's public methods
                std::map<std::string, ClassMethodTable*>& methods = it->second->getDefinedClassMethods();
                std::map<std::string, ClassMethodTable*>::iterator it2;
                for(it2 = methods.begin(); it2 != methods.end(); it2++) {
                    if(!it2->second->getClassMethod()->isProtected()) {
                        // if the method is not protected
                        std::string methodSignature = it2->second->getClassMethod()->getMethodHeader()->methodSignatureAsString();
                        if(methodMapping.count(methodSignature) == 0) {
                            methodMapping[methodSignature] = methodCounter++;
                            numMethods++;
                        }
                    }
                }
            }
        }
    }
}

void ImplInterfaceMethodTableManager::generateTableForClass(CompilationTable* aClass) {
    std::string classCanonicalName = aClass->getCanonicalName();
    // return immediately if already created
    if(tables.count(classCanonicalName) == 1) { return; }
    CompilationTable* superclass = ((ClassTable*) aClass->getSymbolTable())->getClass()->getSuper()->getSuperClassTable();
    ImplInterfaceMethodTable* superclassTable = NULL;
    if(superclass != NULL) {
        // create table first for superclass
        generateTableForClass(superclass);
        superclassTable = tables[superclass->getCanonicalName()];
    }

    tables[classCanonicalName] = new ImplInterfaceMethodTable(classCanonicalName, superclassTable, aClass, numMethods, methodMapping); 
}

void ImplInterfaceMethodTableManager::generateTableForArray() {
    std::string arrayLabel = LabelManager::labelizeForArrays("");
    tables[arrayLabel] = new ImplInterfaceMethodTable(arrayLabel, tables["java.lang.Object"], NULL, numMethods, methodMapping); 
}

unsigned int ImplInterfaceMethodTableManager::getInterfaceMethodIndexInTable(const std::string& methodSignature) {
    // precautionary check
    assert(methodMapping.count(methodSignature) == 1);
    // multiplied by 4 because each entry is 4 bytes
    return methodMapping[methodSignature] * 4;
}

ImplInterfaceMethodTable* ImplInterfaceMethodTableManager::getTableForType(const std::string& type) {
    assert(tables.count(type) == 1);
    return tables[type];
}

ImplInterfaceMethodTable* ImplInterfaceMethodTableManager::getTableForArray() {
    return getTableForType(LabelManager::labelizeForArrays(""));
}
