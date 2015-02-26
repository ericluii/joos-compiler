#include "compilationTable.h"
#include <cassert>

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

// negation is isInterfaceSymbolTable
bool CompilationTable::isClassSymbolTable() {
    return symTable->isClassTable();
}
