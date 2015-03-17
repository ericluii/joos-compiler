#ifndef __BLOCKSTMTS_H__
#define __BLOCKSTMTS_H__

#include "ast.h"

class BlockStmts : public Ast {
    protected:
        BlockStmts* nextBlockStmt;

        // pertaining to reachability stage, by default they are true
        bool reachable;
        bool completion;
    public:
        BlockStmts() : nextBlockStmt(NULL), reachable(true), completion(true) {}
        virtual ~BlockStmts() {
            delete nextBlockStmt;
        }

        BlockStmts* getNextBlockStmt() { return nextBlockStmt; }
        void setNextBlockStmt(BlockStmts* set) { nextBlockStmt = set; }
        bool isLastStatement() { return nextBlockStmt == NULL; }

        bool isReachable() { return reachable; }
        bool canComplete() { return completion; }

        void setReachability(bool set) { reachable = set; }
        void setCompletion(bool set) { completion = set; }
};

#endif
