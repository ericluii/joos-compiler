#ifndef __BLOCKSTMTS_H__
#define __BLOCKSTMTS_H__

#include "ast.h"

class BlockStmts : public Ast {
    protected:
        BlockStmts* nextBlockStmt;
    public:
        BlockStmts() : nextBlockStmt(NULL) {}
        virtual ~BlockStmts() {
            delete nextBlockStmt;
        }

        BlockStmts* getNextBlockStmt() { return nextBlockStmt; }
};

#endif
