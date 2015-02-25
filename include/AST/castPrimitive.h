#ifndef __CASTPRIMITIVE_H__
#define __CASTPRIMITIVE_H__

#include "castExpression.h"
#include "primitiveType.h"

class CastPrimitive : public CastExpression {
    // Rule : CAST_PRIMITIVE
    private:
        PrimitiveType* castType;
    public:
        CastPrimitive(PrimitiveType* castType, Expression* toCast) : CastExpression(toCast), castType(castType) {}
        ~CastPrimitive() {
            delete castType;
            delete toCast;
        }

        PrimitiveType* getPrimitiveTypeToCastTo() { return castType; }
        bool isPrimitiveArrayCast() { return castType->isPrimitiveArray(); }
};

#endif
