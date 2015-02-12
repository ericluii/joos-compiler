#ifndef __SINGLETYPEIMPORTDECL_H__
#define __SINGLETYPEIMPORTDECL_H__

#include "importDecls.h"
#include "name.h"

class SingleTypeImportDecl : public ImportDecls
{       
    public:
        SingleTypeImportDecl(Name *name, ImportDecls *nextImport); 

        Name *GetName();
        ImportDecls *GetNextImport();
};

#endif