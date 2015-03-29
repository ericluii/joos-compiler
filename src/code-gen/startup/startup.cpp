#include <cassert>
#include <fstream>

#include "startup.h"

#include "compilationTable.h"
#include "classTable.h"
#include "interfaceTable.h"
#include "classDecl.h"
#include "interfaceDecl.h"
#include "classMethodTable.h"
#include "classMethod.h"
#include "fieldTable.h"
#include "fieldDecl.h"

class InterfaceMethodTable;

Startup::Startup(std::map<std::string, CompilationTable*>& compilations, CompilationTable* firstUnit) : typeCounter(0), numOfTypes(0),
            interfaceMethodCounter(0), numOfInterfaceMethods(0), compilations(compilations), firstUnit(firstUnit) {
    std::map<std::string, CompilationTable*>::iterator it;
    setAllInterfaceMethods();
    for(it = compilations.begin(); it != compilations.end(); it++) {
        if(it->second->aTypeWasDefined()) {
            // for every type defined, increment by 2
            // one for the type, another for the type array
            numOfTypes+= 2;
        }
    }
    // increment by 5 once more for primitive array types
    numOfTypes+= 5;
}

void Startup::createTablesForCompilation(CompilationTable* table) {
    // build tables
    buildInheritanceTable(table);
    buildInterfaceMethodTable(table);
    buildStaticTable(table);
}

void Startup::createTablesForArrayType() {
    // should be called only after buildTablesForCompilation have been
    // called for all compilation unit
    // array types inheritance
    std::map<std::string, CompilationTable*>::iterator it;
    for(it = compilations.begin(); it != compilations.end(); it++) {
        fillInheritanceTableEntriesForArrays(it->second->getCanonicalName() + ".array");
    }

    for(int i = 0; i < 5; i++) {
        std::string arrayType;
        switch(i) {
            case 0:
                arrayType = "int.array";
                break;
            case 1:
                arrayType = "short.array";
                break;
            case 2:
                arrayType = "byte.array";
                break;
            case 3:
                arrayType = "char.array";
                break;
            case 4:
                arrayType = "boolean.array";
                break;
        }
        fillInheritanceTableEntriesForArrays(arrayType);
    }

    // array types interface method table
    for(unsigned int i = 0; i < numOfInterfaceMethods; i++) {
        interfaceMethodTable[".array"].push_back(NULL);
    }
    copyInterfaceMethodTable(".array", "java.lang.Object");
}

void Startup::fillInheritanceTableEntriesForArrays(const std::string& arrayType) {
    typeMapping[arrayType] = typeCounter++;
    for(unsigned int i = 0; i < numOfTypes; i++) {
        inheritanceTable[arrayType].push_back(false);
    }
    // arrays inherit from java.lang.Object and implements
    // java.io.Serializable and hava.lang.Cloneable
    inheritanceTable[arrayType][typeMapping["java.lang.Object"]] = true;
    inheritanceTable[arrayType][typeMapping["java.lang.Cloneable"]] = true;
    inheritanceTable[arrayType][typeMapping["java.io.Serializable"]] = true;
    // arrays inherit self
    // minus 1 because it was incremented above
    inheritanceTable[arrayType][typeCounter-1] = true;
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

void Startup::buildStaticTable(CompilationTable* table) {
    if(table->aTypeWasDefined()) {
        if(table->isClassSymbolTable()) {
            std::string canonicalName = table->getCanonicalName();
            SymbolTable* symTable = table->getSymbolTable();
            while(symTable != NULL) {
                if(symTable->isFieldTable()) {
                    FieldDecl* field = ((FieldTable*) symTable)->getField();
                    if(field->isStatic() && field->isInitialized()) {
                        // register static methods that have initializers
                        staticTable[canonicalName].push_back((FieldTable*) symTable);
                    }
                }
                symTable = symTable->getNextTable();
            }
        }
    }
}

unsigned int Startup::getIndexOfInterfaceMethodInTable(const std::string& methodSignature) {
    // precautionary check
    assert(interfaceMethodsMapping.count(methodSignature) == 1);
    return interfaceMethodsMapping[methodSignature];
}

unsigned int Startup::getIndexOfTypeInTable(const std::string& typeName) {
    // precautionary check
    assert(typeMapping.count(typeName) == 1);
    return typeMapping[typeName];
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
                std::cout << it->second[i]->generateMethodLabel();
            }
        }
        std::cout << std::endl;
    }
}

void Startup::generateStartupFile() {
    std::ofstream fs("_startup.s");
    // data section
    fs << "section .data\n";
    std::map<std::string, std::vector<bool> >::iterator inheritance;
    for(inheritance = inheritanceTable.begin(); inheritance != inheritanceTable.end(); inheritance++) {
        // create inheritance table
        std::string inheritanceTableName = "INH" + inheritance->first;
        fs << "global " << inheritanceTableName << '\n';
        fs << inheritanceTableName << ": ";
        for(unsigned int i = 0; i < inheritance->second.size(); i++) {
            if(i == 0) { fs << "dd "; }
            else { fs << ", "; }
            fs << inheritance->second[i];
        }
        fs << '\n';
    }

    fs << '\n';
    std::map<std::string, std::vector<ClassMethodTable*> >::iterator interfaces;
    for(interfaces = interfaceMethodTable.begin(); interfaces != interfaceMethodTable.end(); interfaces++) {
        // create interface method table
        std::string interfaceTableName = "INTER" + interfaces->first;
        fs << "global " << interfaceTableName << '\n';
        fs << interfaceTableName << ": ";
        for(unsigned int i = 0; i < interfaces->second.size(); i++) {
            if(i == 0) { fs << "dd "; }
            else { fs << ", "; }
            
            if(interfaces->second[i] == NULL) { fs << "0"; }
            else { fs << interfaces->second[i]->generateMethodLabel(); }
        }
        fs << '\n';
    }

    fs << "\nsection .text\nglobal _start\n_start:\n";

    // call static fields initializer
    std::map<std::string, std::vector<FieldTable*> >::iterator staticFieldsInit;
    for(staticFieldsInit = staticTable.begin(); staticFieldsInit != staticTable.end(); staticFieldsInit++) {
       std::string staticInitCall = "INIT" + staticFieldsInit->first;
       for(unsigned int i =0; i < staticFieldsInit->second.size(); i++) {
           std::string fieldName = staticFieldsInit->second[i]->getField()->getFieldDeclared()->getIdAsString();
           fs << "extern " << staticInitCall << fieldName << '\n';
           fs << "call " << staticInitCall << fieldName << '\n';
       }
    }

    fs << '\n';
    // call static int test() of the first compilation unit
    // given in the command line to joosc
    // fs << "call " << firstUnit->getAClassMethod("test()")->generateMethodLabel() << '\n';
    fs.close();
}
