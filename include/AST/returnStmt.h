#ifndef __RETURNSTMT_H__
#define __RETURNSTMT_H__

#include "blockStmts.h"
#include "expression.h"

class ReturnStmt : public BlockStmts {
    // Rule: STMT_NON_TRAILING_RETURN
    private:
        ExpressionStar* retExpr;
    public:
        ReturnStmt(ExpressionStar* retExpr) : BlockStmts(), retExpr() {}
        ~ReturnStmt() {
            delete retExpr;
        }

        Expression* getReturnExpr() { return retExpr; }
        bool isEmptyReturn() { return retExpr->isEpsilon(); }
};

#endif
