#ifndef __WHILESTMT_H__
#define __WHILESTMT_H__

#include "blockStmts.h"
#include "expression.h"

class WhileStmt : public BlockStmts {
    // Rule: WHILE_UNROLL and NO_SHORT_WHILE_UNROLL
    private:
        Expression* eval;
        BlockStmts* loopStmt;
    public:
        WhileStmt(Expression* eval, BlockStmts* loopStmt) : BlockStmts(), eval(eval), loopStmt(loopStmt) {}
        ~WhileStmt() {
            delete eval;
            delete loopStmt;
        }

        Expression* getExpressionToEvaluate() { return eval; }
        BlockStmts* getLoopStmt() { return loopStmt; }
};

#endif
