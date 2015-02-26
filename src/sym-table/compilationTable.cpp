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

#include "error.h"

CompilationTable::CompilationTable(PackageDecl* package) : package(package), symTable(NULL) {}

CompilationTable::~CompilationTable() {
    delete symTable;
}

SymbolTable* CompilationTable::getSymbolTable() {
    return symTable;
}

std::string CompilationTable::getPackageName() {
    return package->getPackageName()->getFullName();
}

void CompilationTable::setSymbolTable(SymbolTable* set) {
    symTable = set;
}

FieldTable* CompilationTable::getAField(const std::string& field) {
    assert(symTable->isClassTable());
    return fields[field];
 }

ClassMethodTable* CompilationTable::getAClassMethod(const std::string& methodSignature) {
    assert(symTable->isClassTable());
    return classMethods[methodSignature];
}

ConstructorTable* CompilationTable::getAConstructor(const std::string& constructorSignature) {
    assert(symTable->isClassTable());
    return constructors[constructorSignature];
}

bool CompilationTable::checkForFieldPresence(const std::string& field) {
    assert(symTable->isClassTable());
    return fields[field];
}

bool CompilationTable::checkForClassMethodPresence(const std::string& methodSignature) {
    assert(symTable->isClassTable());
    return classMethods.count(methodSignature) == 1;
}
bool CompilationTable::checkForConstructorPresence(const std::string& constructorSignature) {
    assert(symTable->isClassTable());
    return constructors.count(constructorSignature) == 1;
}

// ---------------------------------------------------------------------
// Interface if symbol table is an interface table
InterfaceMethodTable* CompilationTable::getAnInterfaceMethod(const std::string& methodSignature) {
    assert(symTable->isInterfaceTable());
    return interfaceMethods[methodSignature];
}

bool CompilationTable::checkForInterfaceMethodPresence(const std::string& methodSignature) {
    assert(symTable->isInterfaceTable());
    return interfaceMethods.count(methodSignature) == 1;
}

//----------------------------------------------------------------------
// negation is isInterfaceSymbolTable
bool CompilationTable::isClassSymbolTable() {
    return symTable->isClassTable();
}

//----------------------------------------------------------------------
// Symbol table analysis

void CompilationTable::reportError(const std::string& conflict, const std::string& entity, Token* prevToken, Token* currToken) {
    std::stringstream ss;
    ss << conflict << ": '" << entity << "' was previously defined here: "
       << prevToken->getFile() << ":" << prevToken->getLocation().first << ":" << prevToken->getLocation().second;
    Error(E_SYMTABLE, currToken, ss.str());
}

void CompilationTable::registerFormalParameters(ParamList* params, std::map<std::string, Token*>& localVars) {
    while(params != NULL) {
        std::string var = params->getParameterId()->getIdAsString();
        if(localVars.count(var) == 0) {
            // variable is not in the set
            localVars[var] = params->getParameterId()->getToken();
        } else {
            // switched the prevToken and currToken around due to the way
            // we're traversing the AST tree here
            reportError("Formal parameter", var, params->getParameterId()->getToken(), localVars[var]);
        }
        params = params->getNextParameter();
    }
}

void CompilationTable::iterateThroughTable(SymbolTable* table, std::vector<std::map<std::string, Token*>* >& blockScopes) {
    while(table != NULL) {
        if(table->isLocalTable()) {
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

void CompilationTable::checkLocalTable(LocalTable& table, std::vector<std::map<std::string, Token*>* >& blockScopes) {
    Identifier* id = table.getLocalDecl()->getLocalId();
    std::string var = id->getIdAsString();

    for(unsigned int i = 0; i < blockScopes.size(); i++) {
        if(blockScopes[i]->count(var) == 1) {
            // variable is found
            reportError("Local variable", var, (*blockScopes[i])[var], id->getToken());
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
    FormalParamStar* params = methodTable->getClassMethod()->getMethodHeader()->getClassMethodParams();
    registerFormalParameters(params->getListOfParameters(), localVars);
    checkBodyForOverlappingScope(methodTable->getSymbolTableOfMethod(), localVars);
}

void CompilationTable::checkConstructorForOverlappingScope(ConstructorTable* constructorTable) {
    std::map<std::string, Token*> localVars;
    FormalParamStar* params = constructorTable->getConstructor()->getConstructorParameters();
    registerFormalParameters(params->getListOfParameters(), localVars);
    checkBodyForOverlappingScope(constructorTable->getSymbolTableOfConstructor(), localVars);
}

void CompilationTable::checkForOverlappingLocalScope() {
    if(symTable->isClassTable()) {
        std::map<std::string, ClassMethodTable*>::iterator method = classMethods.begin();
        std::map<std::string, ConstructorTable*>::iterator constructor = constructors.begin();

        for(; method != classMethods.end(); method++) {
            if(method->second != NULL) {
                // if there is a body and it's not empty
                checkMethodForOverlappingScope(method->second);
            }
        }

        for(; constructor != constructors.end(); constructor++) {
            if(constructor->second != NULL) {
                checkConstructorForOverlappingScope(constructor->second);
            }
        }
    }
}
