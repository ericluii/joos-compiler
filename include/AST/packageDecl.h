#ifndef __PACKAGEDECL_H__
#define __PACKAGEDECL_H__

#include "ast.h"
#include "name.h"

//This class represents an epsilon
class PackageDecl : public Ast
{
    // Rule: PACKAGE_NAME and PACKAGE_EPSILON
    private:
        Name* package;
    public:
        PackageDecl(Name* package) : package(package) {}
        ~PackageDecl() {
            delete package;
        }

        bool isEpsilon() { return package == NULL; }
};

#endif
