#ifndef __METHODBODY_H__
#define __METHODBODY_H__

#include "ast.h"
#include "blockStmtsStar.h"

class MethodBody : public Ast {
    // Rule: METHOD_BODY and METHOD_BODY_EMPTY
    private:
        BlockStmtsStar* stmts;
        // indicates whether the method body
        // can complete normally or not
        bool completion;
    public:
        MethodBody(BlockStmtsStar* stmts) : stmts(stmts), completion(true) {}
        ~MethodBody() {
            delete stmts;
        }

        BlockStmtsStar* getBlockStmtsStar() { return stmts; }
        bool noDefinition() { return stmts == NULL; }
        bool emptyClassMethodBody() { return stmts->isEpsilon(); }
        void setMethodCompletion(bool set) { completion = set; }
        bool canMethodCompleteNormally() { return completion; }
};

#endif
