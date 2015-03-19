#include <iostream>
#include "constructorTable.h"

ConstructorTable::ConstructorTable(Constructor* constructor, CompilationTable* declaringClass) : SymbolTable(STT_CONSTRUCTOR),
                constructor(constructor), bodyTable(NULL), declaringClass(declaringClass) {}

ConstructorTable::~ConstructorTable() {
    delete bodyTable;
}

Constructor* ConstructorTable::getConstructor() { return constructor; }
SymbolTable* ConstructorTable::getSymbolTableOfConstructor() { return bodyTable; }

CompilationTable* ConstructorTable::getDeclaringClass() { return declaringClass; }
void ConstructorTable::setSymTableOfConstructor(SymbolTable* set) { bodyTable = set; }

void ConstructorTable::printSelf() { 
    std::cout << "Previous: " << prevTable << "| ConstructorTable: " << this << "| AST node: " << constructor
              << "| Next: " << nextTable << std::endl;
}
