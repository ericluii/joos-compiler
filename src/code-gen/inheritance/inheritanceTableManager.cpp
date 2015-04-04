#include <cassert>

#include "inheritanceTable.h"
#include "inheritanceTableManager.h"

// Symbol table
#include "compilationTable.h"
#include "classTable.h"
#include "interfaceTable.h"

// AST
#include "classDecl.h"
#include "interfaceDecl.h"

// Labels
#include "labelManager.h"

InheritanceTableManager::InheritanceTableManager(std::map<std::string, CompilationTable*>& compilations) : typeCounter(0),
            numTypes(5) {
    std::vector<CompilationTable*> definedTypes;
    std::map<std::string, CompilationTable*>::iterator it;
    for(it = compilations.begin(); it != compilations.end(); it++) {
        if(it->second->aTypeWasDefined()) {
            // increment by 2 for each defined type, one for the type
            // the other for the array of the type
            numTypes+= 2;
            definedTypes.push_back(it->second);
        }
    }
    
    for(unsigned int i = 0; i < definedTypes.size(); i++) {
        buildInheritanceTableForCompilation(definedTypes[i]);
    }
    

    for(unsigned int i = 0; i < definedTypes.size(); i++) {
        std::string typeCanonicalName = definedTypes[i]->getCanonicalName();
        buildInheritanceTableForArray(typeCanonicalName + "[]", LabelManager::labelizeForArrays(typeCanonicalName), typeCanonicalName);
    }

    // primitive type arrays
    buildInheritanceTableForArray("int[]", LabelManager::labelizeForArrays("int"), "");
    buildInheritanceTableForArray("short[]", LabelManager::labelizeForArrays("short"), "");
    buildInheritanceTableForArray("byte[]", LabelManager::labelizeForArrays("byte"), "");
    buildInheritanceTableForArray("char[]", LabelManager::labelizeForArrays("char"), "");
    buildInheritanceTableForArray("boolean[]", LabelManager::labelizeForArrays("boolean"), "");

}

InheritanceTableManager::~InheritanceTableManager() {
    std::map<std::string, InheritanceTable*>::iterator it;
    for(it = inheritanceTables.begin(); it != inheritanceTables.end(); it++) {
        delete it->second;
    }
}

void InheritanceTableManager::buildInheritanceTableForCompilation(CompilationTable* type) {
    std::string typeCanonicalName = type->getCanonicalName();
    // return immediately, if inheritance table
    // for this type has been made
    if(inheritanceTables.count(typeCanonicalName) == 1) { return; }
    InheritanceTable* superclassInheritance = NULL;
    if(type->isClassSymbolTable()) {
        // a class
        CompilationTable* superclass = ((ClassTable*) type->getSymbolTable())->getClass()->getSuper()->getSuperClassTable();
        if(superclass != NULL) {
            // build inheritance table first for superclass
            buildInheritanceTableForCompilation(superclass);
            superclassInheritance = inheritanceTables[superclass->getCanonicalName()];
        }
    }

    InheritanceTable* tableForType = new InheritanceTable(typeCanonicalName, superclassInheritance, numTypes);

    Interfaces* interface = NULL;
    if(type->isClassSymbolTable()) {
        interface = ((ClassTable*) type->getSymbolTable())->getClass()->getImplementInterfaces()->getListOfInterfaces();
    } else {
        interface = ((InterfaceTable*) type->getSymbolTable())->getInterface()->getExtendedInterfaces()->getListOfInterfaces();
    }

    while(interface != NULL) {
        CompilationTable* interfaceTable = interface->getImplOrExtInterfaceTable();
        buildInheritanceTableForCompilation(interfaceTable);
        tableForType->pushSuperInterfaceInheritance(inheritanceTables[interfaceTable->getCanonicalName()]);
        interface = interface->getNextInterface();
    }

    // generate inheritance with a particular self index
    typeMapping[typeCanonicalName] = typeCounter;
    tableForType->generateInheritance(typeCounter++);

    inheritanceTables[typeCanonicalName] = tableForType;
}

void InheritanceTableManager::buildInheritanceTableForArray(const std::string& arrayType, const std::string& tableName,
            const std::string& canonicalName) {
    InheritanceTable* inh = NULL;
    if(canonicalName == "") {
        // canonical name not given because it's an array of primitive type
        inh = new InheritanceTable(tableName, inheritanceTables["java.lang.Object"], numTypes);
    } else {
        // canonical name is given because it's an array of reference type
        inh = new InheritanceTable(tableName, inheritanceTables[canonicalName], numTypes);
    }

    inh->pushSuperInterfaceInheritance(inheritanceTables["java.lang.Cloneable"]);
    inh->pushSuperInterfaceInheritance(inheritanceTables["java.io.Serializable"]);

    typeMapping[arrayType] = typeCounter;
    inh->generateInheritance(typeCounter++);
    
    inheritanceTables[arrayType] = inh;
}

unsigned int InheritanceTableManager::getTypeMapping(const std::string& type) {
    // precautionary check
    assert(typeMapping.count(type) == 1);
    return typeMapping[type];
}

InheritanceTable* InheritanceTableManager::getTableForType(const std::string& type) {
    // precautionary check
    assert(inheritanceTables.count(type) == 1);
    return inheritanceTables[type];
}
