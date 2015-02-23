#ifndef __EXPRESSIONSTAR_H__
#define __EXPRESSIONSTAR_H__

#include "ast.h"
#include "expression.h"

class ExpressionStar : public Ast {
    // Rule: EXPR_STAR and EXPR_STAR_EPSILON
    private:
        Expression* expr;
    public:
        ExpressionStar(Expression* expr) : expr(expr) {}
        ~ExpressionStar() {
            delete expr;
        }

        Expression* getExpression() { return expr; }
        bool isEpsilon() { return expr == NULL; }
};

#endif
