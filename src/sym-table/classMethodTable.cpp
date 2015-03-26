#include <iostream>
#include "classMethodTable.h"

ClassMethodTable::ClassMethodTable(ClassMethod* method, CompilationTable* declaringClass) : SymbolTable(STT_CLASSMETHOD), method(method),
                bodyTable(NULL), declaringClass(declaringClass) {}
ClassMethodTable::~ClassMethodTable() {
    delete bodyTable;
}

ClassMethod* ClassMethodTable::getClassMethod() { return method; }
SymbolTable* ClassMethodTable::getSymbolTableOfMethod() { return bodyTable; }

CompilationTable* ClassMethodTable::getDeclaringClass() { return declaringClass; }
void ClassMethodTable::setSymTableOfMethod(SymbolTable* set) { bodyTable = set; }

void ClassMethodTable::printSelf() {
    std::cout << "Previous: " << prevTable << "| ClassMethodTable: " << this << "| AST node: " << method 
              << "| Next: " << nextTable << std::endl;
}
