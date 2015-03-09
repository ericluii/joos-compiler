#ifndef __PRIMARYNEWARRAY_H__
#define __PRIMARYNEWARRAY_H__

#include "primary.h"
#include "expression.h"
#include "type.h"

class PrimaryNewArray : public Primary {
    // Rule: MAKE_NEW_PRIMITIVE_ARRAY and MAKE_NEW_NONPRIMITIVE_ARRAY
    private:
        Type* type;
        Expression* dimensionExpr;
    public:
        PrimaryNewArray(Type* type, Expression* dimensionExpr) : Primary(), type(type), dimensionExpr(dimensionExpr) {}
        virtual ~PrimaryNewArray() {
            delete type;
            delete dimensionExpr;
        }

        Type* getArrayType() { return type; }
        Expression* getTheDimension() { return dimensionExpr; }
};

#endif
