#ifndef __IMPORTDECLSSTAR_H__
#define __IMPORTDECLSSTAR_H__

#include "ast.h"
#include "importDecls.h"

//this class represents an epsilon
class ImportDeclsStar : public Ast
{
    // Rule: IMPORT_STAR_DECLS and IMPORT_STAR_EPSILON
    private:
        ImportDecls* imports;
    public:
        ImportDeclsStar(ImportDecls* imports) : imports(imports) {}
        ~ImportDeclsStar() {
            delete imports;
        }
        
        ImportDecls* getImportDeclarations() { return imports; }
        bool isEpsilon() { return imports == NULL; }
};

#endif
