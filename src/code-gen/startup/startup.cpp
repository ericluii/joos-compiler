#include "startup.h"

#include "compilationTable.h"
#include "classTable.h"
#include "interfaceTable.h"
#include "classDecl.h"
#include "interfaceDecl.h"

Startup::Startup(std::map<std::string, CompilationTable*>& compilations) : typeCounter(0), numOfTypes(0),
            compilations(compilations) {
    std::map<std::string, CompilationTable*>::iterator it;
    for(it = compilations.begin(); it != compilations.end(); it++) {
        if(it->second->aTypeWasDefined()) {
            // for every type defined, increment
            numOfTypes++;
        }
    }
    // increment once more for array types
    numOfTypes++;
}

void Startup::buildTables() {
    std::map<std::string, CompilationTable*>::iterator it;
    for(it = compilations.begin(); it != compilations.end(); it++) {
        buildInheritanceTable(it->second);
    }
   
    // array types inheritance
    typeMapping[".array"] = typeCounter;
    for(unsigned int i = 0; i < numOfTypes; i++) {
        inheritanceTable[".array"].push_back(false);
    }
    // arrays inherit from java.lang.Object
    inheritanceTable[".array"][typeMapping["java.lang.Object"]] = true;
    // arrays inherit self
    inheritanceTable[".array"][typeCounter] = true;
    // printInheritanceTable();
}

void Startup::copyInheritanceTable(const std::string& targetName, const std::string& sourceName) {
    for(unsigned int i = 0; i < numOfTypes; i++) {
        if(!inheritanceTable[targetName][i]) {
            // only if inheritance entry is false, so that inheritance information
            // isn't overriden
            inheritanceTable[targetName][i] = inheritanceTable[sourceName][i];
        }
    }
}

void Startup::printInheritanceTable() {
    std::map<std::string, std::vector<bool> >::iterator it;
    for(it = inheritanceTable.begin(); it != inheritanceTable.end(); it++) {
        std::cout << it->first;
        for(unsigned int i = 0; i < numOfTypes; i++) {
            std::cout << " " << it->second[i];
        }
        std::cout << std::endl;
    }
}

void Startup::buildInheritanceTable(CompilationTable* table) {
    if(table->aTypeWasDefined()) {
        std::string canonicalName = table->getCanonicalName();
        // return imediately if inheritance table have been created
        // for this type
        if(inheritanceTable.count(canonicalName) == 1) { return; }

        for(unsigned int i = 0; i < numOfTypes; i++) {
            // by default set everything to false
            inheritanceTable[canonicalName].push_back(false);
        }

        // give a mapping for this type
        typeMapping[canonicalName] = typeCounter;
        typeCounter++;

        if(table->isClassSymbolTable()) {
            ClassDecl* aClass = ((ClassTable*) table->getSymbolTable())->getClass();
            Super* superClass = aClass->getSuper();
            if(!superClass->isEpsilon() || superClass->isImplicitlyExtending()) {
                // create inheritance table for superclass
                buildInheritanceTable(aClass->getSuper()->getSuperClassTable());
                std::string superClassCanonicalName = aClass->getSuper()->getSuperClassTable()->getCanonicalName();
                // copy inheritance table from superclass
                copyInheritanceTable(canonicalName, superClassCanonicalName);
            }
        }
       
        Interfaces* implOrExtInterface = NULL;
        if(table->isClassSymbolTable()) {
            ClassDecl* aClass = ((ClassTable*) table->getSymbolTable())->getClass();
            implOrExtInterface = aClass->getImplementInterfaces()->getListOfInterfaces();
        } else {
            InterfaceDecl* anInterface = ((InterfaceTable*) table->getSymbolTable())->getInterface();
            implOrExtInterface = anInterface->getExtendedInterfaces()->getListOfInterfaces();
        }

        while(implOrExtInterface != NULL) {
            // for each superinterface
            // create inheritance table for superinterfaces
            buildInheritanceTable(implOrExtInterface->getImplOrExtInterfaceTable());
            std::string superInterfaceCanonicalName = implOrExtInterface->getImplOrExtInterfaceTable()->getCanonicalName();
            // copy inheritance table from superinterfaces
            copyInheritanceTable(canonicalName, superInterfaceCanonicalName);
            implOrExtInterface = implOrExtInterface->getNextInterface();
        }

        // establish inheritance for self
        inheritanceTable[canonicalName][typeMapping[canonicalName]] = true;
    }
}
