#ifndef __BLOCKSTMTSSTAR_H__
#define __BLOCKSTMTSSTAR_H__

#include "ast.h"
#include "blockStmts.h"

class BlockStmtsStar : public Ast {
    private:
        BlockStmts* stmts;
    public:
        BlockStmts(BlockStmts* stmts) : stmts(stmts) {}
        ~BlockStmts() {
            delete stmts;
        }

        bool emptyBlock() { return stmts == NULL; }
};

#endif
