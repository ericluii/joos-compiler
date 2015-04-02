#include<cassert>

#include "objectLayout.h"
#include "objectLayoutManager.h"

// Symbol table
#include "compilationTable.h"
#include "classTable.h"

// AST
#include "classDecl.h"

ObjectLayoutManager::ObjectLayoutManager(std::map<std::string, CompilationTable*>& compilations) {
    std::map<std::string, CompilationTable*>::iterator it;
    for(it = compilations.begin(); it != compilations.end(); it++) {
        if(it->second->aTypeWasDefined() && it->second->isClassSymbolTable()) {
            // the type is a class if a type is defined
            createLayoutForClass(it->second);
        }
    }
}

ObjectLayoutManager::~ObjectLayoutManager() {
    std::map<CompilationTable*, ObjectLayout*>::iterator it;
    for(it = layouts.begin(); it != layouts.end(); it++) {
        delete it->second;
    }
}

void ObjectLayoutManager::createLayoutForClass(CompilationTable* aClass) {
    // layout of class has been created
    if(layouts.count(aClass) == 1) { return; }
    
    CompilationTable* superclass = ((ClassTable*) aClass->getSymbolTable())->getClass()->getSuper()->getSuperClassTable();
    ObjectLayout* parentLayout = NULL;
    if(superclass != NULL) {
        createLayoutForClass(superclass);
        parentLayout = layouts[superclass];
    }

    layouts[aClass] = new ObjectLayout(parentLayout, aClass);
}

ObjectLayout* ObjectLayoutManager::getLayoutForClass(CompilationTable* aClass) {
    // precautionary check
    assert(layouts.count(aClass) == 1);
    return layouts[aClass];
}
