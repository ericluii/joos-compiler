#ifndef __RETURNSTMT_H__
#define __RETURNSTMT_H__

#include "blockStmts.h"
#include "expressionStar.h"

class ReturnStmt : public BlockStmts {
    // Rule: STMT_NON_TRAILING_RETURN
    private:
        ExpressionStar* retExpr;
    public:
        ReturnStmt(ExpressionStar* retExpr) : BlockStmts(), retExpr(retExpr) {}
        ~ReturnStmt() {
            delete retExpr;
        }

        ExpressionStar* getReturnExpr() { return retExpr; }
        bool isEmptyReturn() { return retExpr->isEpsilon(); }
};

#endif
