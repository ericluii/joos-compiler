#ifndef __EMPTYSTMT_H__
#define __EMPTYSTMT_H__

#include "blockStmts.h"

class EmptyStmt : public BlockStmts {
    public:
        EmptyStmt() : BlockStmts() {}
        ~EmptyStmt() {}
};

#endif
