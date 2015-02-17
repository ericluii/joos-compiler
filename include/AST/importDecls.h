#ifndef __IMPORTDECLS_H__
#define __IMPORTDECLS_H__

#include "ast.h"
#include "name.h"

//interface definition for import declarations
class ImportDecls : public Ast
{
    private:
        ImportDecls* nextImport;
        Name *import;
    public:
        ImportDecls(Name* import) : nextImport(NULL), import(import) {}
        ~ImportDecls() {
            delete import;
            delete nextImport;
        }

        Name *GetImport() { return import; }
        ImportDecls *GetNextImport() { return nextImport; }
        void setNextImport(ImportDecls* set) { nextImport = set; }
};

#endif
