#ifndef __IMPORTDECLSSTAR_H__
#define __IMPORTDECLSSTAR_H__

#include "ast.h"
#include "importDecls.h"

//this class represents an epsilon
class ImportDeclsStar : public Ast
{
    private:
        ImportDecls* imports;
    public:
        ImportDeclsStar(ImportDecls* imports) : imports(imports) {}
        bool isEpsilon() { return imports == NULL; }
};

#endif
