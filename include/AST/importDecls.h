#ifndef __IMPORTDECLS_H__
#define __IMPORTDECLS_H__

#include "importDeclsStar.h"
#include "name.h"

//interface definition for import declarations
class ImportDecls : public ImportDeclsStar
{
    protected:
        Name *name;
        ImportDecls *nextImport;
        
    public:
        ImportDecls(){}
        
        virtual Name *GetName() = 0;
        virtual ImportDecls *GetNextImport() = 0;
};

#endif