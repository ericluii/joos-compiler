#ifndef __ARRAYACCESSPRIMARY_H__
#define __ARRAYACCESSPRIMARY_H__

#include "arrayAccess.h"
#include "primary.h"

class ArrayAccessPrimary : public ArrayAccess {
    private:
        Primary* arrayPrimary;
    public:
        ArrayAccessPrimary(Primary* arrayPrimary, Expression* accessExpr) : ArrayAccess(accessExpr), arrayPrimary(arrayPrimary) {}
        ~ArrayAccessPrimary() {
            delete arrayPrimary;
        }

        Primary* getArrayPrimary() { return arrayPrimary; }
};

#endif
