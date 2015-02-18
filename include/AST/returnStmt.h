#ifndef __RETURNSTMT_H__
#define __RETURNSTMT_H__

#include "blockStmts.h"
#include "expression.h"

class ReturnStmt : BlockStmts {
    private:
        Expression* retExpr;
    public:
        ReturnStmt(Expression* retExpr) : BlockStmts(), retExpr() {}
        ~ReturnStmt() {
            delete retExpr;
        }

        Expression* getReturnExpr() { return retExpr; }
        bool isEmptyReturn() { return retExpr == NULL; }
};

#endif
