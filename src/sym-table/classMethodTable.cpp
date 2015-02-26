#include <iostream>
#include "classMethodTable.h"

ClassMethodTable::ClassMethodTable(ClassMethod* method) : SymbolTable(STT_CLASSMETHOD), method(method), bodyTable(NULL) {}
ClassMethodTable::~ClassMethodTable() {
    delete bodyTable;
}

ClassMethod* ClassMethodTable::getClassMethod() { return method; }
SymbolTable* ClassMethodTable::getSymbolTableOfMethod() { return bodyTable; }
void ClassMethodTable::setSymTableOfMethod(SymbolTable* set) { bodyTable = set; }

void ClassMethodTable::printSelf() {
    std::cout << "Previous: " << prevTable << "| ClassMethodTable: " << this << "| AST node: " << method 
              << "| Next: " << nextTable << std::endl;
}

