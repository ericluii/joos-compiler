#ifndef __EXPRESSIONSTAR_H__
#define __EXPRESSIONSTAR_H__

#include "ast.h"
#include "expression.h"

class ExpressionStar : public Expression {
    private:
        Expression* expr;
    public:
        ExpressionStar(Expression* expr) : expr(expr) {}
        ~ExpressionStar() {
            delete expr;
        }

        Expression* getExpr() { return expr; }
        bool isEpsilon() { return expr == NULL; }
};

#endif
