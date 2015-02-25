#ifndef __STMTEXPRASSIGN_H__
#define __STMTEXPRASSIGN_H__

#include "stmtExpr.h"
#include "assignment.h"

class StmtExprAssign : public StmtExpr {
    // Rule: STMTEXPR_TO_ASSIGN
    private:
        Assignment* assign;
    public:
        StmtExprAssign(Assignment* assign) : StmtExpr(), assign(assign) {}
        ~StmtExprAssign() {
            delete assign;
        }

        Assignment* getAssignmentExpression() { return assign; }
};

#endif
