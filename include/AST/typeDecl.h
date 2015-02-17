#ifndef __TYPEDECL_H__
#define __TYPEDECL_H__

#include "ast.h"

//this class represents an epsilon
class TypeDecl : public Ast
{
    public:
        TypeDecl() {}
        virtual ~TypeDecl() {}
        virtual bool isEpsilon() { return true; }
};

#endif
