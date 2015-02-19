#ifndef __NESTEDBLOCK_H__
#define __NESTEDBLOCK_H__

#include "blockStmts.h"
#include "blockStmtsStar.h"

class NestedBlock : public BlockStmts {
    // Rule: STMT_NON_TRAILING_BLOCK
    private:
        BlockStmtsStar* nestedBlk;
    public:
        NestedBlock(BlockStmtsStar* nestedBlk) : BlockStmts(), nestedBlk(nestedBlk) {}
        ~NestedBlock() {
            delete nestedBlk;
        }

        BlockStmtsStar* getBlock() { return nestedBlk; }
};

#endif
