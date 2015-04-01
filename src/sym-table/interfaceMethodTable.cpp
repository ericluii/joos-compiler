#include <iostream>
#include "interfaceMethodTable.h"
#include "interfaceMethod.h"
#include "compilationTable.h"

InterfaceMethodTable::InterfaceMethodTable(InterfaceMethod* method, CompilationTable* declaringInterface) :
        SymbolTable(STT_INTERFACEMETHOD), method(method), declaringInterface(declaringInterface) {}
InterfaceMethodTable::~InterfaceMethodTable() {}

InterfaceMethod* InterfaceMethodTable::getInterfaceMethod() { return method; }

CompilationTable* InterfaceMethodTable::getDeclaringInterface() { return declaringInterface; }
void InterfaceMethodTable::printSelf() {
    std::cout << "Previous: " << prevTable << "| InterfaceMethodTable: " << this << "| AST node: " << method 
              << "| Next: " << nextTable << std::endl;
}
