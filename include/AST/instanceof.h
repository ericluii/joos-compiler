#ifndef __INSTANCEOF_H__
#define __INSTANCEOF_H__

#include "expression.h"
#include "type.h"

class InstanceOf : public Expression {
    // Rule: RELATION_TO_INSTANCEOF
    private:
        Expression* expr;
        Type* instanceType;
    public:
        InstanceOf(Expression* expr, Type* instanceType) : expr(expr), instanceType(instanceType) {}
        ~InstanceOf() {
            delete expr;
            delete instanceType;
        }

        Expression* getExpressionToCheck() { return expr; }
        Type* getTypeToCheck() { return instanceType; }
};

#endif
