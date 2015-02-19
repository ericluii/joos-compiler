#ifndef __ARRAYACCESSNAME_H__
#define __ARRAYACCESSNAME_H__

#include "arrayAccess.h"
#include "name.h"

class ArrayAccessName : public ArrayAccess {
    // Rule: ACCESS_ARRAY_NAME
    private:
        Name* arrayName;
    public:
        ArrayAccessName(Name* arrayName, Expression* accessExpr) : ArrayAccess(accessExpr), arrayName(arrayName) {}
        ~ArrayAccessName() {
            delete arrayName;
        }

        Name* getArrayName() { return arrayName; }
};

#endif
