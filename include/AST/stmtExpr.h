#ifndef __STMTEXPR_H__
#define __STMTEXPR_H__

#include "blockStmts.h"

class StmtExpr : public BlockStmts {
    public:
        StmtExpr() : BlockStmts() {}
        virtual ~StmtExpr() {}
};

#endif
