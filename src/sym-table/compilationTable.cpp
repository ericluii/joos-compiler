#include <iostream>
#include <cassert>
#include <sstream>
#include "compilationTable.h"
#include "localTable.h"
#include "forTable.h"
#include "nestedBlockTable.h"
#include "classMethod.h"
#include "constructor.h"
#include "interfaceMethod.h"
#include "localDecl.h"
#include "fieldDecl.h"
#include "paramList.h"

#include "classTable.h"
#include "fieldTable.h"
#include "paramTable.h"
#include "interfaceTable.h"
#include "classDecl.h"
#include "interfaceDecl.h"
#include "error.h"

CompilationTable::CompilationTable(PackageDecl* package, const std::string& filename, CompilationUnit* unit) : package(package),
                symTable(NULL), filename(filename), unit(unit), extendFromObject(NULL), established(false), compilationsInPackage(NULL) {}

CompilationTable::~CompilationTable() {
    delete symTable;
}

SymbolTable* CompilationTable::getSymbolTable() {
    return symTable;
}

std::string CompilationTable::getPackageName() {
    if(!package->isEpsilon()) {
        return package->getPackageName()->getFullName();
    }
    // default package name
    return "";
}

PackageDecl* CompilationTable::getPackageRawForm() {
    return package;
}

std::string CompilationTable::getClassOrInterfaceName() {
    if(symTable == NULL) {
        return "";
    }

    if(symTable->isClassTable()) {
        return ((ClassTable*) symTable)->getClass()->getClassId()->getIdAsString();
    }
    return ((InterfaceTable*) symTable)->getInterface()->getInterfaceId()->getIdAsString();
}

std::string CompilationTable::getCanonicalName() {
    std::string packageName = getPackageName();
    if(packageName != "") {
        packageName+= '.';
    }

    return packageName + getClassOrInterfaceName();
}

std::string CompilationTable::getFilename() {
    return filename;
}

CompilationUnit* CompilationTable::getCompilationUnit() {
    return unit;
}

void CompilationTable::setSymbolTable(SymbolTable* set) {
    symTable = set;
}

void CompilationTable::setCompilationsInPackage(std::vector<CompilationTable*>* tables) {
    compilationsInPackage = tables;
}

void CompilationTable::setASingleTypeImport(const std::string& typeName, CompilationTable* table, Token* importTok) {
    if(singleTypeImports.count(typeName) == 0) {
        singleTypeImports[typeName] = table;
    } else {
        if(singleTypeImports[typeName] != table) {
            // if it's not actually a repeating import of the same type
            std::stringstream ss;
            ss << "Single type import '" << table->getPackageName() << "." << typeName << "' conflicts with single type import '"
               << singleTypeImports[typeName]->getCanonicalName() << "'.";
            Error(E_TYPELINKING, importTok, ss.str());
        }
    }
}

void CompilationTable::setAnImportTypeOnDemand(const std::string& packageName, std::vector<CompilationTable*>* compilations) {
    if(importsOnDemand.count(packageName) == 0) {
        // if the package has not yet been specified
        importsOnDemand[packageName] = compilations;
    }
}

void CompilationTable::checkForConflictingCanonicalName() {
    // should only be invoked after compilationsInPackage have been set
    assert(compilationsInPackage != NULL);
    for(unsigned int i = 0; i < compilationsInPackage->size(); i++) {
        if(compilationsInPackage->at(i) != this) {
            // if not equal to self
            std::string canonicalName = getCanonicalName();
            if(canonicalName == compilationsInPackage->at(i)->getCanonicalName()) {
                std::stringstream ss;
                ss << filename << ": error: Canonical name '" << canonicalName << "' has already been used in file '"
                   << compilationsInPackage->at(i)->getFilename() << '.';
                Error(E_DEFAULT, NULL, ss.str());
            }
        }
    }
}

FieldTable* CompilationTable::getAField(const std::string& field) {
    assert(symTable->isClassTable());
    if(fields.count(field) == 1) {
        return fields[field];
    }
    // field does not exist
    return NULL;
 }

ClassMethodTable* CompilationTable::getAClassMethod(const std::string& methodSignature) {
    assert(symTable->isClassTable());
    if(classMethods.count(methodSignature) == 1) {
        return classMethods[methodSignature];
    }
    // method does not exist
    return NULL;
}

ConstructorTable* CompilationTable::getAConstructor(const std::string& constructorSignature) {
    assert(symTable->isClassTable());
    if(constructors.count(constructorSignature)) {
        return constructors[constructorSignature];
    }
    // constructor does not exist
    return NULL;
}

void CompilationTable::registerAField(const std::string& field, FieldTable* table) {
    // called only during the symbol table creation stage
    if(fields.count(field) == 0) {
        fields[field] = table;
    } else {
        // field is already present
        std::stringstream ss;
        Token* prevField = fields[field]->getField()->getFieldDeclared()->getToken();
        ss << "Field '" << field << "' was previously defined here: "
           << prevField->getFile() << ":" << prevField->getLocation().first << ":" << prevField->getLocation().second;
        Error(E_SYMTABLE, table->getField()->getFieldDeclared()->getToken(), ss.str());
    }
}

void CompilationTable::registerClassMethodsAndConstructors() {
    // called after the hierarchy checking stage
    // assume that the hierarchy checking stage has checked for duplicate methods
    SymbolTable* table = symTable;
    if(table != NULL) {
        // a type was defined
        // precautionary check that the symbol table is indeed a class table
        assert(symTable->isClassTable());
        table = table->getNextTable();
        while(table != NULL) {
            if(table->isClassMethodTable()) {
                ClassMethodTable* methodTable = (ClassMethodTable*) table;
                classMethods[methodTable->getClassMethod()->getMethodHeader()->methodSignatureAsString()] = methodTable;
            } else if(table->isConstructorTable()) {
                ConstructorTable* ctorTable = (ConstructorTable*) table;
                constructors[ctorTable->getConstructor()->constructorSignatureAsString()] = ctorTable;
            }

            table = table->getNextTable();
        }
    }
}

// To be called after hierarchy checking
void CompilationTable::registerInheritedField(const std::string& field, FieldTable* table) {
    if(fields.count(field) == 0) {
        // is not overriden
        fields[field] = table;
    }
}

void CompilationTable::registerInheritedClassMethod(const std::string& methodSignature, ClassMethodTable* table) {
    if(classMethods.count(methodSignature) == 0) {
        // is not overriden
        classMethods[methodSignature] = table;
    }
}

void CompilationTable::registerInheritedInterfaceMethod(const std::string& methodSignature, InterfaceMethodTable* table) {
    if(interfaceMethods.count(methodSignature) == 0) {
        // is not overriden
        interfaceMethods[methodSignature] = table;
    }
}

bool CompilationTable::isInheritanceEstablished() {
    // precautionary check that this is called for a class table
    return established;
}

void CompilationTable::inheritClassFieldsAndMethods() {
    // precautionary check that the symbol table is indeed a class table
    assert(symTable->isClassTable());
    std::map<std::string, FieldTable*>::iterator fieldIt;
    std::map<std::string, ClassMethodTable*>::iterator methodIt;
    CompilationTable* parent = ((ClassTable*) symTable)->getClass()->getSuper()->getSuperClassTable();
    if(parent != NULL) {
        // if there was actually a superclass inherited
        // only case there is none is for java.lang.Object
        for(fieldIt = parent->fields.begin(); fieldIt != parent->fields.end(); fieldIt++) {
            registerInheritedField(fieldIt->second->getField()->getFieldDeclared()->getIdAsString(), fieldIt->second);
        }

        for(methodIt = parent->classMethods.begin(); methodIt != parent->classMethods.end(); methodIt++) {
            registerInheritedClassMethod(methodIt->second->getClassMethod()->getMethodHeader()->methodSignatureAsString(),
                                         methodIt->second);
        }
    }
    // indicate that inheritance have been properly done
    established = true;
}

void CompilationTable::inheritInterfaceMethods(CompilationTable* object) {
    // precautionary check that the symbol table is indeed an interface table
    assert(!symTable->isClassTable());
    std::map<std::string, InterfaceMethodTable*>::iterator methodIt;
    InterfaceDecl* anInterface = ((InterfaceTable*) symTable)->getInterface();
    if(!anInterface->noExtendedInterfaces()) {
        Interfaces* extended = anInterface->getExtendedInterfaces()->getListOfInterfaces();
        while(extended != NULL) {
            CompilationTable* parent = extended->getImplOrExtInterfaceTable();
            for(methodIt = parent->interfaceMethods.begin(); methodIt != parent->interfaceMethods.end(); methodIt++) {
                registerInheritedInterfaceMethod(methodIt->second->getInterfaceMethod()->methodSignatureAsString(),
                                        methodIt->second);
            }
            extended = extended->getNextInterface();
        }
    }
    // automatically make interface inherit from object
    extendFromObject = object;
    established = true;
}

// ---------------------------------------------------------------------
// Interface if symbol table is an interface table
InterfaceMethodTable* CompilationTable::getAnInterfaceMethod(const std::string& methodSignature) {
    assert(symTable->isInterfaceTable());
    if(interfaceMethods.count(methodSignature) == 1) {
        return interfaceMethods[methodSignature];
    }
    return NULL;
}

ClassMethodTable* CompilationTable::getAnInterfaceMethodFromObject(const std::string& methodSignature) {
    assert(symTable->isInterfaceTable());
    // make sure this is called only when the method can't be found
    // from the interface itself
    assert(interfaceMethods.count(methodSignature) == 0);
    return extendFromObject->getAClassMethod(methodSignature);
}

void CompilationTable::registerInterfaceMethods() {
    // assumptions hold the same way as registerClassMethodsAndConstructor
    if(symTable != NULL) {
        // a type was defined
        // precautionary check that the symbol table is indeed an interface table
        assert(symTable->isInterfaceTable());
        // by default we know the next symbol table must be an InterfaceMethodTable
        InterfaceMethodTable* methodTable = (InterfaceMethodTable*) symTable->getNextTable();
        while(methodTable != NULL) {
            interfaceMethods[methodTable->getInterfaceMethod()->methodSignatureAsString()] = methodTable;
            methodTable = (InterfaceMethodTable*) methodTable->getNextTable();
        }
    }
}

//----------------------------------------------------------------------
// Interface to check if symbol table is NULL
bool CompilationTable::aTypeWasDefined() { return symTable != NULL; }

//----------------------------------------------------------------------
// negation is isInterfaceSymbolTable
bool CompilationTable::isClassSymbolTable() {
    return symTable->isClassTable();
}

//----------------------------------------------------------------------
// Symbol table analysis

void CompilationTable::reportLocalError(const std::string& conflict, const std::string& entity, Token* prevToken, Token* currToken) {
    std::stringstream ss;
    ss << conflict << ": '" << entity << "' was previously defined here: "
       << prevToken->getFile() << ":" << prevToken->getLocation().first << ":" << prevToken->getLocation().second;
    Error(E_SYMTABLE, currToken, ss.str());
}

void CompilationTable::iterateThroughTable(SymbolTable* table, std::vector<std::map<std::string, Token*>* >& blockScopes) {
    while(table != NULL) {
        if(table->isParamTable()) {
            checkParamTable(*((ParamTable*) table), blockScopes);
        } else if(table->isLocalTable()) {
            checkLocalTable(*((LocalTable*) table), blockScopes);
        } else if(table->isNestedBlockTable()) {
            checkNestedBlockTable(*((NestedBlockTable*) table), blockScopes);
        } else {
            // For table
            checkForTable(*((ForTable*) table), blockScopes);
        }
        table = table->getNextTable();
    }
}

void CompilationTable::checkParamTable(ParamTable& table, std::vector<std::map<std::string, Token*>* >& blockScopes) {
    Identifier* id = table.getParameter()->getParameterId();
    std::string param = id->getIdAsString();

    for(unsigned int i = 0; i < blockScopes.size(); i++) {
        if(blockScopes[i]->count(param) == 1) {
            // parameter is found
            reportLocalError("Formal parameter", param, (*blockScopes[i])[param], id->getToken());
            return;
        }
    }

    // always add to the last mapping since this variable
    // in the currently inspected block
    (*blockScopes[blockScopes.size() - 1])[param] = id->getToken();
}

void CompilationTable::checkLocalTable(LocalTable& table, std::vector<std::map<std::string, Token*>* >& blockScopes) {
    Identifier* id = table.getLocalDecl()->getLocalId();
    std::string var = id->getIdAsString();

    for(unsigned int i = 0; i < blockScopes.size(); i++) {
        if(blockScopes[i]->count(var) == 1) {
            // variable is found
            reportLocalError("Local variable", var, (*blockScopes[i])[var], id->getToken());
            return;
        }
    }

    // always add to the last mapping since this variable
    // in the currently inspected block
    (*blockScopes[blockScopes.size() - 1])[var] = id->getToken();
}

void CompilationTable::checkNestedBlockTable(NestedBlockTable& table, std::vector<std::map<std::string, Token*>* >& blockScopes) {
    SymbolTable* blockTable = table.getSymbolTableOfBlock();
    std::map<std::string, Token*>* newBlock = new std::map<std::string, Token*>();
    blockScopes.push_back(newBlock);
    if(blockTable != NULL) {
        iterateThroughTable(blockTable, blockScopes);
    }

    // makes sure that the back of the vector is the same as the previously
    // created map
    assert(blockScopes.back() == newBlock);
    blockScopes.pop_back();
}

void CompilationTable::checkForTable(ForTable& table, std::vector<std::map<std::string, Token*>* >& blockScopes) {
    LocalTable* initTable = table.getForInitTable();
    SymbolTable* loopTable = table.getLoopTable();
    std::map<std::string, Token*>* newBlock = NULL;
    if(initTable != NULL) {
        newBlock = new std::map<std::string, Token*>();
        // creates a new scope specifically for forInit, important
        // since the declaration in forInit should not ve visible outside of the for statement
        blockScopes.push_back(newBlock);
        iterateThroughTable(initTable, blockScopes);
    } else if(loopTable != NULL) {
        // if initTable is NULL then loopTable will need to be checked through here
        // since initTable wouldn't be pointing to loopTable
        iterateThroughTable(loopTable, blockScopes);
    }

    if(newBlock != NULL) {
        // if newBlock was actually allocated
        assert(blockScopes.back() == newBlock);
        blockScopes.pop_back();
    }
}

void CompilationTable::checkBodyForOverlappingScope(SymbolTable* body, std::map<std::string, Token*>& localVars) {
    // a vector of scopes to simulate nested block scopes
    std::vector<std::map<std::string, Token*>* > blockScopes;
    blockScopes.push_back(&localVars);
    iterateThroughTable(body, blockScopes);
}

void CompilationTable::checkMethodForOverlappingScope(ClassMethodTable* methodTable) {
    std::map<std::string, Token*> localVars;
    checkBodyForOverlappingScope(methodTable->getSymbolTableOfMethod(), localVars);
}

void CompilationTable::checkConstructorForOverlappingScope(ConstructorTable* constructorTable) {
    std::map<std::string, Token*> localVars;
    checkBodyForOverlappingScope(constructorTable->getSymbolTableOfConstructor(), localVars);
}

void CompilationTable::checkForOverlappingLocalScope() {
    if(symTable != NULL) {
        // a type was defined
        if(symTable->isClassTable()) {
            // the type is a class
            SymbolTable* table = symTable;
            while(table != NULL) {
                if(table->isClassMethodTable()) {
                    checkMethodForOverlappingScope((ClassMethodTable*) table);
                } else if(table->isConstructorTable()) {
                    checkConstructorForOverlappingScope((ConstructorTable*) table);
                }
                table = table->getNextTable();
            }
        }
    }
}

CompilationTable* CompilationTable::checkTypePresenceFromSingleImport(const std::string& typeName) {
    if(singleTypeImports.count(typeName) == 1) {
        return singleTypeImports[typeName];
    }

    return NULL;
}

CompilationTable* CompilationTable::checkTypePresenceInPackage(const std::string& typeName) {
    for(unsigned int i = 0; i < compilationsInPackage->size(); i++) {
        if(compilationsInPackage->at(i)->getClassOrInterfaceName() == typeName) {
            // safe since we have checked for conflicting canonical name
            return compilationsInPackage->at(i);
        }
    }

    return NULL;
}

CompilationTable* CompilationTable::checkTypePresenceFromImportOnDemand(const std::string& typeName, Token* tokName) {
    bool found = false;
    CompilationTable* retTable = NULL;
    std::map<std::string, std::vector<CompilationTable*>* >::iterator it;
    for(it = importsOnDemand.begin(); it != importsOnDemand.end(); it++) {
        std::vector<CompilationTable*>::iterator it2;
        for(it2 = it->second->begin(); it2 != it->second->end(); it2++) {
            if(typeName == (*it2)->getClassOrInterfaceName()) {
                if(found) {
                    std::stringstream ss;
                    ss << "Type specified by simple name '" << typeName
                       << "' can be found through import type on demand '" << retTable->getPackageName()
                       << "' and '" << (*it2)->getPackageName() << "'.";
                    Error(E_TYPELINKING, tokName, ss.str());
                    return retTable;
                }
                found = true;
                retTable = *it2;
            }
        }
    }
    return retTable;
}
