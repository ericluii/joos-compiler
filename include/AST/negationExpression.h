#ifndef __NEGATIONEXPRESSION_H__
#define __NEGATIONEXPRESSION_H__

#include <string>
#include "expression.h"

class NegationExpression : public Expression {
    // Rule: NEG_UNARY and NOT_UNARY
    private:
        Expression* negate;
    public:
        NegationExpression(Expression* negate) : Expression(), negate(negate) {}
        ~NegationExpression() {
            delete negate;
        }

        Expression* getNegatedExpression() { return negate; }
};

#endif
