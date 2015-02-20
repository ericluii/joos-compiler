#ifndef __BLOCKSTMTSSTAR_H__
#define __BLOCKSTMTSSTAR_H__

#include "ast.h"
#include "blockStmts.h"

class BlockStmtsStar : public Ast {
    // Rule: BLOCK_STMTS and BLOCK_STMTS_EPSILON
    private:
        BlockStmts* stmts;
    public:
        BlockStmtsStar(BlockStmts* stmts) : stmts(stmts) {}
        ~BlockStmtsStar() {
            delete stmts;
        }

        BlockStmts* getStatements() { return stmts; }
        bool isEpsilon() { return stmts == NULL; }
};

#endif
