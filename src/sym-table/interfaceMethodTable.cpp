#include <iostream>
#include "interfaceMethodTable.h"

InterfaceMethodTable::InterfaceMethodTable(InterfaceMethod* method) : SymbolTable(STT_INTERFACEMETHOD), method(method) {}
InterfaceMethodTable::~InterfaceMethodTable() {}

InterfaceMethod* InterfaceMethodTable::getInterfaceMethod() { return method; }
void InterfaceMethodTable::printSelf() {
    std::cout << "Previous: " << prevTable << "| InterfaceMethodTable: " << this << "| AST node: " << method 
              << "| Next: " << nextTable << std::endl;
}

