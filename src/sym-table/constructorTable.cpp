#include <iostream>
#include "constructorTable.h"

ConstructorTable::ConstructorTable(Constructor* constructor) : SymbolTable(STT_CONSTRUCTOR), constructor(constructor),
                                                               bodyTable(NULL) {}

ConstructorTable::~ConstructorTable() {
    delete bodyTable;
}

Constructor* ConstructorTable::getConstructor() { return constructor; }
SymbolTable* ConstructorTable::getSymbolTableOfConstructor() { return bodyTable; } 
void ConstructorTable::setSymTableOfConstructor(SymbolTable* set) { bodyTable = set; }

void ConstructorTable::printSelf() { 
    std::cout << "Previous: " << prevTable << "| ConstructorTable: " << this << "| AST node: " << constructor
              << "| Next: " << nextTable << std::endl;
}
