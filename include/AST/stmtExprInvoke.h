#ifndef __STMTEXPRINVOKE_H__
#define __STMTEXPRINVOKE_H__

#include "stmtExpr.h"
#include "methodInvoke.h"

class StmtExprInvoke : public StmtExpr {
    // Rule: STMTEXPR_INVOKE
    private:
        MethodInvoke* invoke;
    public:
        StmtExprInvoke(MethodInvoke* invoke) : StmtExpr(), invoke(invoke) {}
        ~StmtExprInvoke() {
            delete invoke;
        }

        MethodInvoke* getMethod() { return invoke; }
};

#endif
