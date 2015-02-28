#include <iostream>
#include "localTable.h"

LocalTable::LocalTable(LocalDecl* local) : SymbolTable(STT_LOCAL), local(local) {}
LocalTable::~LocalTable() {}

LocalDecl* LocalTable::getLocalDecl() { return local; }
void LocalTable::printSelf() {
    std::cout << "Previous: " << prevTable << "| LocalTable: " << this << "| AST node: " << local 
              << "| Next: " << nextTable << std::endl;
}

