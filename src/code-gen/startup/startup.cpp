#include "startup.h"

#include "compilationTable.h"
#include "classTable.h"
#include "interfaceTable.h"
#include "classDecl.h"
#include "interfaceDecl.h"
#include "classMethodTable.h"
#include "classMethod.h"

class InterfaceMethodTable;

Startup::Startup(std::map<std::string, CompilationTable*>& compilations) : typeCounter(0), numOfTypes(0),
            interfaceMethodCounter(0), numOfInterfaceMethods(0), compilations(compilations) {
    std::map<std::string, CompilationTable*>::iterator it;
    setAllInterfaceMethods();
    for(it = compilations.begin(); it != compilations.end(); it++) {
        if(it->second->aTypeWasDefined()) {
            // for every type defined, increment
            numOfTypes++;
            if(it->second->getCanonicalName() == "java.lang.Object") {
                object = it->second;
            }
        }
    }
    // increment once more for array types
    numOfTypes++;
}

void Startup::buildTables() {
    // array types inheritance
    typeMapping[".array"] = typeCounter;
    for(unsigned int i = 0; i < numOfTypes; i++) {
        inheritanceTable[".array"].push_back(false);
    }
    // arrays inherit from java.lang.Object and implements
    // java.io.Serializable and java.lang.Cloneable
    inheritanceTable[".array"][typeMapping["java.lang.Object"]] = true;
    inheritanceTable[".array"][typeMapping["java.lang.Cloneable"]] = true;
    inheritanceTable[".array"][typeMapping["java.io.Serializable"]] = true;
    // arrays inherit self
    inheritanceTable[".array"][typeCounter] = true;
    
    std::map<std::string, CompilationTable*>::iterator it;
    for(it = compilations.begin(); it != compilations.end(); it++) {
        // preserve this order
        buildInheritanceTable(it->second);
        buildInterfaceMethodTable(it->second);
    }

    // array types interface method table
    for(unsigned int i = 0; i < numOfInterfaceMethods; i++) {
        interfaceMethodTable[".array"].push_back(NULL);
    }

    copyInterfaceMethodTable(".array", "java.lang.Object");
    // printInheritanceTable();
    // printInterfaceMethodTable();
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

void Startup::copyInterfaceMethodTable(const std::string& targetName, const std::string& sourceName) {
    for(unsigned int i = 0; i < numOfInterfaceMethods; i++) {
        if(interfaceMethodTable[targetName][i] == NULL) {
            // don't override non-NULL entries
            interfaceMethodTable[targetName][i] = interfaceMethodTable[sourceName][i];
        }
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

void Startup::setAllInterfaceMethods() {
    std::map<std::string, CompilationTable*>::iterator it;
    for(it = compilations.begin(); it!= compilations.end(); it++) {
        if(it->second->aTypeWasDefined()) {
            if(!it->second->isClassSymbolTable()) {
                // an interface
                CompilationTable* interface = it->second;
                std::map<std::string, InterfaceMethodTable*>& methods = interface->getDefinedInterfaceMethods();
                std::map<std::string, InterfaceMethodTable*>::iterator it2;
                for(it2 = methods.begin(); it2 != methods.end(); it2++) {
                    std::string methodSignature = it2->first;
                    if(interfaceMethodsMapping.count(methodSignature) == 0) {
                        // not in the mapping, add to it and increase counter
                        interfaceMethodsMapping[methodSignature] = interfaceMethodCounter;
                        interfaceMethodCounter++;
                    }
                    interfaceMethodsDeclaredIn[methodSignature].push_back(interface);
                }
                numOfInterfaceMethods+= interface->getNumDefinedInterfaceMethods();
            }

            // special case, since interface 'inherits' from java.lang.Object
            if(it->second->getCanonicalName() == "java.lang.Object") {
                CompilationTable* object = it->second;
                std::map<std::string, ClassMethodTable*>& methods = object->getDefinedClassMethods();
                std::map<std::string, ClassMethodTable*>::iterator it2;
                for(it2 = methods.begin(); it2 != methods.end(); it2++) {
                    std::string methodSignature = it2->first;
                    if(interfaceMethodsMapping.count(methodSignature) == 0) {
                        interfaceMethodsMapping[methodSignature] = interfaceMethodCounter;
                        interfaceMethodCounter++;
                    }
                    interfaceMethodsDeclaredIn[methodSignature].push_back(object);
                }
                numOfInterfaceMethods+= object->getNumDefinedClassMethods();
            }
        }
    }
}

void Startup::buildInterfaceMethodTable(CompilationTable* table) {
    if(table->aTypeWasDefined()) {
        if(table->isClassSymbolTable()) {
            // it's a class
            std::string canonicalName = table->getCanonicalName();
            // immediately return if the interface method table for this
            // class has been created already
            if(interfaceMethodTable.count(canonicalName) == 1) { return; }
           
            // initialize first for this class
            for(unsigned int i = 0; i < numOfInterfaceMethods; i++) {
                interfaceMethodTable[canonicalName].push_back(NULL);
            }

            Super* superClass = ((ClassTable*) table->getSymbolTable())->getClass()->getSuper();
            if(!superClass->isEpsilon() || superClass->isImplicitlyExtending()) {
                // recurse first for superclass, including for java.lang.Object
                // even though java.lang.Object doesn't really implement interface
                // classes, but it's still needed
                buildInterfaceMethodTable(superClass->getSuperClassTable());
                std::string superClassCanonicalName = superClass->getSuperClassTable()->getCanonicalName();
                copyInterfaceMethodTable(canonicalName, superClassCanonicalName);
            }

            std::map<std::string, ClassMethodTable*>& methods = table->getDefinedClassMethods();
            std::map<std::string, ClassMethodTable*>::iterator it;
            for(it = methods.begin(); it != methods.end(); it++) {
                std::string methodSignature = it->first;
                if(interfaceMethodsMapping.count(methodSignature) == 1) {
                    // if the method can be found in the interface methods mapping
                    std::vector<CompilationTable*>::iterator it2;
                    for(it2 = interfaceMethodsDeclaredIn[methodSignature].begin();
                        it2 != interfaceMethodsDeclaredIn[methodSignature].end(); it2++) {
                        std::string targetCanonicalName = (*it2)->getCanonicalName();
                        if(inheritanceTable[canonicalName][typeMapping[targetCanonicalName]]) {
                            // if current class is a subclass of one of the interfaces that
                            // declares this method then register this class method into the interface method table
                            interfaceMethodTable[canonicalName][interfaceMethodsMapping[methodSignature]] = it->second;
                            // early break
                            break;
                        }
                    }
                }
            }
        }
    }
}

// -----------------------------------------------------------------------------------
// miscellaneous

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

void Startup::printInterfaceMethodTable() {
    std::map<std::string, std::vector<ClassMethodTable*> >::iterator it;
    for(it = interfaceMethodTable.begin(); it != interfaceMethodTable.end(); it++) {
        std::cout << it->first;
        for(unsigned int i = 0; i < numOfInterfaceMethods; i++) {
            std::cout << " ";
            if(it->second[i] == NULL) {
                std::cout << "NULL";
            } else {
                std::cout << it->second[i]->getClassMethod()->getMethodHeader()->methodSignatureAsString();
            }
        }
        std::cout << std::endl;
    }
}
