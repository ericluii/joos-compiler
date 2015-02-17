#ifndef __PRIMARYEXPR_H__
#define __PRIMARYEXPR_H__

#include "primary.h"
#include "expression.h"

class PrimaryExpr : public Primary {
    private:
        Expression* expr;
    public:
        PrimaryExpr(Expression* expr) : expr(expr) {}
        ~PrimaryExpr() {
            delete expr;
        }

        Expression* getExpr() { return expr; }
};


#endif
