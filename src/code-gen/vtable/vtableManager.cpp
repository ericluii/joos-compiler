#include <cassert>

#include "vtableManager.h"
#include "vtableLayout.h"
#include "compilationTable.h"
#include "classTable.h"
#include "classDecl.h"

// Label
#include "labelManager.h"

VTableManager::VTableManager(std::map<std::string, CompilationTable*>& compilations) {
    std::map<std::string, CompilationTable*>::iterator it;
    for(it = compilations.begin(); it != compilations.end(); it++) {
        if(it->second->aTypeWasDefined() && it->second->isClassSymbolTable()) {
            createVTableLayoutForCompilation(it->second);
        }
    }

    createVTableLayoutForArrays();
}

VTableManager::~VTableManager() {
    std::map<std::string, VTableLayout*>::iterator it;
    for(it = vTableCollection.begin(); it != vTableCollection.end(); it++) {
        delete it->second;
    }
}

void VTableManager::createVTableLayoutForCompilation(CompilationTable* table) {
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

void VTableManager::createVTableLayoutForArrays() {
    std::map<std::string, CompilationTable*>::iterator it;
    VTableLayout* ObjectVTable = vTableCollection["java.lang.Object"];
    vTableCollection[LabelManager::labelizeForArrays("")] = new VTableLayout(ObjectVTable);
}

VTableLayout* VTableManager::getVTableLayoutForType(const std::string& typeName) {
    // precautionary check
    assert(vTableCollection.count(typeName) == 1);
    return vTableCollection[typeName];
}

VTableLayout* VTableManager::getVTableLayoutForArray() {
    return getVTableLayoutForType(LabelManager::labelizeForArrays(""));
}
