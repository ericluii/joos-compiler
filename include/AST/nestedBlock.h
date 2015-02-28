#ifndef __NESTEDBLOCK_H__
#define __NESTEDBLOCK_H__

#include "blockStmts.h"
#include "blockStmtsStar.h"
#include "nestedBlockTable.h"

class NestedBlock : public BlockStmts {
    // Rule: STMT_NON_TRAILING_BLOCK
    private:
        BlockStmtsStar* nestedBlk;
        NestedBlockTable* table;
    public:
        NestedBlock(BlockStmtsStar* nestedBlk) : BlockStmts(), nestedBlk(nestedBlk), table(NULL) {}
        ~NestedBlock() {
            delete nestedBlk;
        }

        BlockStmtsStar* getNestedBlock() { return nestedBlk; }
        bool isEmptyNestedBlock() { return nestedBlk->isEpsilon(); }
        void setNestedBlockTable(NestedBlockTable* set) { table = set; }
        NestedBlockTable* getNestedBlockTable() { return table; }
};

#endif
