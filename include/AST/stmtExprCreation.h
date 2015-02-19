#ifndef __STMTEXPRCREATION_H__
#define __STMTEXPRCREATION_H__

#include "stmtExpr.h"
#include "newClassCreation.h"

class StmtExprCreation : public StmtExpr {
    // Rule: STMTEXPR_MAKE_CLASS
    private:
        NewClassCreation* create;
    public:
        StmtExprCreation(NewClassCreation* create) : StmtExpr(), create(create) {}
        ~StmtExprCreation() {
            delete create;
        }

        Assignment* getCreatedClass() { return create; }
};

#endif
