#include <iostream>
#include "interfaceTable.h"

InterfaceTable::InterfaceTable(InterfaceDecl* declaration) : SymbolTable(STT_INTERFACE), declaration(declaration) {}
InterfaceTable::~InterfaceTable() {}

InterfaceDecl* InterfaceTable::getInterface() { return declaration; }
void InterfaceTable::printSelf() {
    std::cout << "InterfaceTable: " << this << "| AST node: " << declaration
              << "| Next: " << nextTable << std::endl;
}
