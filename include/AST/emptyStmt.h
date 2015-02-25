#ifndef __EMPTYSTMT_H__
#define __EMPTYSTMT_H__

#include "blockStmts.h"

class EmptyStmt : public BlockStmts {
    // Rule: STMT_NON_TRAILING_EMPTY
    public:
        EmptyStmt() : BlockStmts() {}
        ~EmptyStmt() {}
};

#endif
