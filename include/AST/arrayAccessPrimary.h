#ifndef __ARRAYACCESSPRIMARY_H__
#define __ARRAYACCESSPRIMARY_H__

#include "arrayAccess.h"
#include "primary.h"

class ArrayAccessPrimary : public ArrayAccess {
    // Rule: ACCESS_ARRAY_PRIMARY
    private:
        Primary* arrayPrimary;
    public:
        ArrayAccessPrimary(Primary* arrayPrimary, Expression* accessExpr) : ArrayAccess(accessExpr), arrayPrimary(arrayPrimary) {}
        ~ArrayAccessPrimary() {
            delete arrayPrimary;
        }

        Primary* getAccessedPrimaryArray() { return arrayPrimary; }
};

#endif
