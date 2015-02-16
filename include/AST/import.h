#ifndef __IMPORTDECL_H__
#define __IMPORTDECL_H__

#include "importDecls.h"
#include "name.h"

class ImportDecl : public ImportDecls
{       
    public:
        ImportDecl(Name *name) : ImportDecls(name) {} 
};

#endif
