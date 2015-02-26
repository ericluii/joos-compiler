#include <iostream>
#include "fieldTable.h"

FieldTable::FieldTable(FieldDecl* field) : SymbolTable(STT_FIELD), field(field) {}
FieldTable::~FieldTable() {}

FieldDecl* FieldTable::getField() { return field; }
void FieldTable::printSelf() {
    std::cout << "Previous: " << prevTable << "| FieldTable: " << this << "| AST node: " << field 
              << "| Next: " << nextTable << std::endl;
}
