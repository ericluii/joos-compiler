#ifndef __IMPORTDECLS_H__
#define __IMPORTDECLS_H__

#include "ast.h"
#include "name.h"

//interface definition for import declarations
class ImportDecls : public Ast
{
    // Rule: SINGLE_TYPE_IMPORT and IMPORT_ON_DEMAND
    private:
        ImportDecls* nextImport;
        Name *import;
    public:
        ImportDecls(Name* import) : nextImport(NULL), import(import) {}
        ~ImportDecls() {
            delete import;
            delete nextImport;
        }

        Name *getCurrentImport() { return import; }
        ImportDecls *getNextImport() { return nextImport; }
        void setNextImport(ImportDecls* set) { nextImport = set; }
        bool isLastImport() { return nextImport == NULL; }
};

#endif
