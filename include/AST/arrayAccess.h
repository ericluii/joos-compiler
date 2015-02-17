#ifndef __ARRAYACCESS_H__
#define __ARRAYACCESS_H__

#include "ast.h"
#include "expression.h"

class ArrayAccess : public Primary {
    protected:
        Expression* accessExpr;
    public:
        ArrayAccess(Expression* accessExpr) : accessExpr(accessExpr) {}
        virtual ~ArrayAccess() {
            delete accessExpr;
        }

        Expression* getAccessExpr() { return accessExpr; }
};

#endif
