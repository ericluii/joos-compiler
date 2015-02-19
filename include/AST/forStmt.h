#ifndef __FORSTMT_H__
#define __FORSTMT_H__

#include "blockStmts.h"
#include "expressionStar.h"
#include "stmtExpr.h"

class ForStmt : public BlockStmts {
    // Rule: FOR_STMT and NO_SHORT_FOR
    private:
        BlockStmts* forInit;
        ExpressionStar* eval;
        StmtExpr* forUpdate;
        BlockStmts* loopStmt;
    public:
        ForStmt(BlockStmts* forInit, ExpressionStar* eval, StmtExpr* forUpdate, BlockStmts* loopStmt) : BlockStmts(), forInit(forInit),
                eval(eval), forUpdate(forUpdate), loopStmt(loopStmt) {}
        ~ForStmt() {
            delete forInit;
            delete eval;
            delete forUpdate;
            delete loopStmt;
        }

        BlockStmts* getForInit() { return forInit; }
        ExpressionStar* getExpressionToEvaluate() { return eval; }
        StmtExpr* getForUpdate() { return forUpdate; }
        BlockStmts* getLoopStmt() { return loopStmt; }

        bool emptyForInit() { return forInit == NULL; }
        bool emptyExpression() { return eval->isEpsilon(); }
        bool emptyUpdate() { return forUpdate == NULL; }
};

#endif
