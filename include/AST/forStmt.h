#ifndef __FORSTMT_H__
#define __FORSTMT_H__

#include "blockStmts.h"
#include "expression.h"
#include "stmtExpr.h"

class ForStmt : public BlockStmts {
    // Rule: FOR_STMT and NO_SHORT_FOR
    private:
        BlockStmts* forInit;
        Expression* eval;
        StmtExpr* forUpdate;
        BlockStmts* loopStmt;
    public:
        ForStmt(BlockStmts* forInit, Expression* eval, StmtExpr* forUpdate, BlockStmts* loopStmt) : BlockStmts(), forInit(forInit),
                eval(eval), forUpdate(forUpdate), loopStmt(loopStmt) {}
        ~ForStmt() {
            delete forInit;
            delete eval;
            delete forUpdate;
        }

        BlockStmts* getForInit() { return forInit; }
        Expression* getExpressionToEvaluate() { return eval; }
        StmtExpr* getForUpdate() { return forUpdate; }
        BlockStmts* getLoopStmt() { return loopStmt; }

        bool emptyForInit() { return forInit == NULL; }
        bool emptyExpression() { return eval == NULL; }
        bool emptyUpdate() { return forUpdate == NULL; }
};

#endif
