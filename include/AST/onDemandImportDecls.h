#ifndef __ONDEMANDIMPORTDECL_H__
#define __ONDEMANDIMPORTDECL_H__

#include "importDecls.h"
#include "name.h"

class OnDemandImportDecl : public ImportDecls
{       
    public:
        OnDemandImportDecl(Name *name, ImportDecls *nextImport); 

        Name *GetName();
        ImportDecls *GetNextImport();
};

#endif