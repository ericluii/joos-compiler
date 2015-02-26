#include <iostream>
#include "nestedBlockTable.h"

NestedBlockTable::NestedBlockTable(NestedBlock* block) : SymbolTable(STT_NESTEDBLOCK),
                                                         setTable(false), block(block), blockTable(NULL) {}

NestedBlockTable::~NestedBlockTable() {
    delete blockTable;
}

NestedBlock* NestedBlockTable::getNestedBlock() { return block; }
SymbolTable* NestedBlockTable::getSymbolTableOfBlock() { return blockTable; }
void NestedBlockTable::setSymTableOfBlock(SymbolTable* set) { blockTable = set; }
void NestedBlockTable::printSelf() {
    std::cout << "Previous: " << prevTable << "| NestedBlockTable: " << this << "| AST node: " << block
              << "| Next: " << nextTable << std::endl;
}

void NestedBlockTable::indicateTableIsSet() { setTable = true; }
bool NestedBlockTable::isTableSet() { return setTable; }

