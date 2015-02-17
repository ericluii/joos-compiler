#ifndef __PRIMARYNEWARRAY_H__
#define __PRIMARYNEWARRAY_H__

#include "primary.h"
#include "expression.h"
#include "type.h"

class PrimaryNewArray : public Primary {
    private:
        Type* type;
        Expression* dimensionExpr;
    public:
        PrimaryNewArray(Type* type, Expression* dimensionExpr) : type(type), dimensionExpr(dimensionExpr) {}
        virtual ~PrimaryNewArray() {
            delete type;
            delete dimensionExpr;
        }

        Type* getArrayType() { return type; }
        Expression* getDimensionExpr() { return dimensionExpr; }
};

#endif
