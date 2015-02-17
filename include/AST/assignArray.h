#ifndef __ASSIGNARRAY_H__
#define __ASSIGNARRAY_H__

#include "expression.h"
#include "arrayAccess.h"

class AssignArray : public Assignment {
    private:
        ArrayAccess* LHS;
    public:
        AssignArray(ArrayAccess* LHSarray, Expression* RHSexpr) : Assignment(RHSexpr), LHS(LHSarray) {}
        ~AssignArray() {
            delete LHS;
        }

        ArrayAccess* getLHSArray() { return LHS; }
};

#endif
