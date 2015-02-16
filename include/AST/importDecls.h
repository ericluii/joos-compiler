#ifndef __IMPORTDECLS_H__
#define __IMPORTDECLS_H__

#include "ast.h"
#include "name.h"

//interface definition for import declarations
class ImportDecls : public Ast
{
    private:
        ImportDecls* nextImport;
    protected:
        Name *import;
    public:
        ImportDecls(Name* import) : nextImport(NULL), import(import) {}
        
        Name *GetImport() { return import; }
        ImportDecls *GetNextImport() { return nextImport; }
        void setNextImport(ImportDecls* set) { nextImport = set; }
};

#endif
