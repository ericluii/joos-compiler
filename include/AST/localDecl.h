#ifndef __LOCALDECL_H__
#define __LOCALDECL_H__

#include "blockStmts.h"
#include "type.h"
#include "identifier.h"
#include "expression.h"

class LocalDecl : public BlockStmts {
    // Rule: LOCAL_VAR_DECL
    private:
        Type* type;
        Identifier* id;
        Expression* initExpr;
    public:
        LocalDecl(Type* type, Identifier* id, Expression* initExpr) : BlockStmts(), type(type), id(id), initExpr(initExpr) {}
        ~LocalDecl() {
            delete type;
            delete id;
            delete initExpr;
        }

        Type* getLocalType() { return type; }
        Identifier* getLocalId() { return id; }
        Expression* getLocalInitExpr() { return initExpr; }
};

#endif
