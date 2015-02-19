#ifndef __CASTEXPRESSION_H__
#define __CASTEXPRESSION_H__

#include "expression.h"

class CastExpression : public Expression {
    protected:
        Expression* toCast;
    public:
        CastExpression() : toCast(toCast) {}
        virtual ~CastExpression() {
            delete toCast;
        }

        Expression* getExpressionToCast() { return toCast; }
};

#endif
