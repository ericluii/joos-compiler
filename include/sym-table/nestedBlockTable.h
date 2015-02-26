#ifndef __NESTEDBLOCKTABLE_H__
#define __NESTEDBLOCKTABLE_H__

#include "symbolTable.h"

class NestedBlock;

// Represents the block of an if stmt, while stmt and a nested block
class NestedBlockTable : public SymbolTable {
    private:
        bool setTable;
        NestedBlock* block;
        SymbolTable* blockTable;
    public:
        NestedBlockTable(NestedBlock* block);
        ~NestedBlockTable(); 

        NestedBlock* getNestedBlock(); 
        SymbolTable* getSymbolTableOfBlock();
        void setSymTableOfBlock(SymbolTable* set);
        void printSelf(); 

        void indicateTableIsSet();
        bool isTableSet();
};

#endif
