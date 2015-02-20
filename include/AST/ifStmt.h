#ifndef __IFSTMT_H__
#define __IFSTMT_H__

#include "blockStmts.h"

class IfStmt : public BlockStmts {
    // Rule: IF_STMT, IF_THEN_STMT and NO_SHORT_IF_THEN
    private:
        Expression* eval;
        BlockStmts* exTrue;
        BlockStmts* exFalse;
    public:
        IfStmt(Expression* eval, BlockStmts* exTrue, BlockStmts* exFalse) : BlockStmts(), eval(eval), exTrue(exTrue), exFalse(exFalse) {}
        ~IfStmt() {
            delete eval;
            delete exTrue;
            delete exFalse;
        }

        Expression* getExpressionToEvaluate() { return eval; }
        BlockStmts* getExecuteTruePart() { return exTrue; }
        BlockStmts* getExecuteFalsePart() { return exFalse; }

        bool noElsePart() { return exFalse == NULL; }
};

#endif
