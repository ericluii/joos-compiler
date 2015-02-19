#ifndef __ASSIGNFIELD_H__
#define __ASSIGNFIELD_H__

#include "expression.h"
#include "fieldAccess.h"

class AssignField : public Assignment {
    // Rule: LVALUE_FIELD_ACCESS
    private:
        FieldAccess* LHS;
    public:
        AssignField(FieldAccess* LHSfield, Expression* RHSexpr) : Assignment(RHSexpr), LHS(LHSfield) {}
        ~AssignField() {
            delete LHS;
        }

        FieldAccess* getLHSField() { return LHS; }
};

#endif
