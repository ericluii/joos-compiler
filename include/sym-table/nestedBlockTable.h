#ifndef __NESTEDBLOCKTABLE_H__
#define __NESTEDBLOCKTABLE_H__

#include "symbolTable.h"

class NestedBlock;

// Represents the block of an if stmt, while stmt and a nested block
class NestedBlockTable : public SymbolTable {
    private:
        NestedBlock* block;
        SymbolTable* blockTable;
    public:
        NestedBlockTable(NestedBlock* block) : SymbolTable(STT_NESTEDBLOCK), block(block), blockTable(NULL) {}
        ~NestedBlockTable() {}

        NestedBlock* getNestedBlock() { return block; }
        SymbolTable* getSymbolTableOfBlock() { return blockTable; }
        void setSymTableOfBlock(SymbolTable* set) { blockTable = set; }
};

#endif
