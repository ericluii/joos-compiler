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
    vTableCollection[".array"] = new VTableLayout(ObjectVTable);
}

VTableLayout* VTableManager::getVTableLayoutOfType(const std::string& typeName) {
    // precautionary check
    assert(vTableCollection.count(typeName) == 1);
    return vTableCollection[typeName];
}
