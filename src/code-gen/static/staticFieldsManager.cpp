#include <cassert>

#include "staticFieldsManager.h"
#include "compilationTable.h"
#include "staticFields.h"

StaticFieldsManager::StaticFieldsManager(std::map<std::string, CompilationTable*>& compilations) {
    std::map<std::string, CompilationTable*>::iterator it;
    for(it = compilations.begin(); it != compilations.end(); it++) {
        if(it->second->aTypeWasDefined() && it->second->isClassSymbolTable()) {
            classStatics[it->second] = new StaticFields(it->second);
        }
    }
}

StaticFieldsManager::~StaticFieldsManager() {
    std::map<CompilationTable*, StaticFields*>::iterator it;
    for(it = classStatics.begin(); it != classStatics.end(); it++) {
        delete it->second;
    }
}

StaticFields* StaticFieldsManager::getStaticFieldsForClass(CompilationTable* aClass) {
    assert(classStatics.count(aClass) == 1);
    return classStatics[aClass];
}
