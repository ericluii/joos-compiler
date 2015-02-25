#ifndef __CASTNAME_H__
#define __CASTNAME_H__

#include "castExpression.h"
#include "name.h"

class CastName : public CastExpression {
    // Rule: CAST_NONPRIMITIVE and CAST_TO_EXPRESSION
    private:
        Name* castType;
    public:
        CastName(Name* castType, Expression* toCast) : CastExpression(toCast), castType(castType) {}
        ~CastName() {
            delete castType;
        }

        Name* getNameToCastTo() { return castType; }
};

#endif
