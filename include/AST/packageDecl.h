#ifndef __PACKAGEDECL_H__
#define __PACKAGEDECL_H__

#include "ast.h"

//This class represents an epsilon
class PackageDecl : public Ast
{
    private:
        Name* package;
    public:
        PackageDecl(Name* package) : package(package) {}
        bool isEpsilon() { return package == NULL; }
};

#endif
