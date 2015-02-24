#ifndef __NESTEDBLOCKTABLE_H__
#define __NESTEDBLOCKTABLE_H__

#include "symbolTable.h"

class NestedBlock;

class NestedBlockTable : public SymbolTable {
    private:
        NestedBlock* block;
        SymbolTable* blockTable;
    public:
        NestedBlockTable(NestedBlock* block) : SymbolTable(STT_NESTEDBLOCK), block(block), blockTable(NULL) {}
        ~NestedBlock() {}

        NestedBlock* getNestedBlock() { return block; }
        SymbolTable* getSymbolTableOfBlock() { return blockTable; }
        void setSymTableofBlock(SymbolTable* set) { blockTable = set; }
};

#endif
