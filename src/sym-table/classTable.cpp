#include <iostream>
#include "classTable.h"

ClassTable::ClassTable(ClassDecl* declaration) : SymbolTable(STT_CLASS), declaration(declaration) {}
ClassTable::~ClassTable() {}

ClassDecl* ClassTable::getClass() { return declaration; }
void ClassTable::printSelf() {
    std::cout << "ClassTable: " << this << "| AST node: " << declaration
              << "| Next: " << nextTable << std::endl;
}
