#include <cassert>

#include "vtableManager.h"
#include "vtableLayout.h"
#include "compilationTable.h"
#include "classTable.h"
#include "classDecl.h"

VTableManager::VTableManager(std::map<std::string, CompilationTable*>& compilations) : compilations(compilations) {}

VTableManager::~VTableManager() {
    std::map<std::string, VTableLayout*>::iterator it;
    for(it = vTableCollection.begin(); it != vTableCollection.end(); it++) {
        delete it->second;
    }
}

void VTableManager::createVTableLayoutForCompilation(CompilationTable* table) {
    if(table->aTypeWasDefined()) {
        if(table->isClassSymbolTable()) {
            std::string canonicalName = table->getCanonicalName();
            // check if vtable for this class has been created or not
            if(vTableCollection.count(canonicalName) == 1) {
                // if so return immediately
                return;
            }
            ClassDecl* aClass = ((ClassTable*) table->getSymbolTable())->getClass();
            CompilationTable* superClass = aClass->getSuper()->getSuperClassTable();
            if(superClass != NULL) {
                // if there is a superclass, then make the vtable for the superclass first
                createVTableLayoutForCompilation(superClass);
                vTableCollection[canonicalName] = new VTableLayout(table,
                                                      vTableCollection[superClass->getCanonicalName()]);
            } else {
                vTableCollection[canonicalName] = new VTableLayout(table, NULL);
            }

        }
    }
}

void VTableManager::createVTableLayoutForArrays() {
    std::map<std::string, CompilationTable*>::iterator it;
    VTableLayout* ObjectVTable = vTableCollection["java.lang.Object"];
    std::string typeArrayName;
    VTableLayout* newVTableLayout;
    for(it = compilations.begin(); it != compilations.end(); it++) {
        // for each type that can possibly be an array
        typeArrayName = it->second->getCanonicalName() + "[]";
        newVTableLayout = new VTableLayout(typeArrayName, ObjectVTable);
        newVTableLayout->createVTableForArray();
        vTableCollection[typeArrayName] = newVTableLayout;
    }
    
    // primitive array types
    for(unsigned int i = 0; i < 5; i++) {
        switch(i) {
            case 0:
                typeArrayName = "int[]";
                break;
            case 1:
                typeArrayName = "short[]";
                break;
            case 2:
                typeArrayName = "byte[]";
                break;
            case 3:
                typeArrayName = "char[]";
                break;
            case 4:
                typeArrayName = "boolean[]";
                break;
        }
        newVTableLayout = new VTableLayout(typeArrayName, ObjectVTable);
        newVTableLayout->createVTableForArray();
        vTableCollection[typeArrayName] = newVTableLayout;
    }
}

VTableLayout* VTableManager::getVTableLayoutOfType(const std::string& typeName) {
    // precautionary check
    assert(vTableCollection.count(typeName) == 1);
    return vTableCollection[typeName];
}
