#ifndef __INTERFACEDECL_H__
#define __INTERFACEDECL_H__

#include "typeDecl.h"

class InterfaceDecl : public TypeDecl
{
        //fields
    public:
        InterfaceDecl(/*parameters*/) {} 

        bool isEpsilon() { return false; }
        //get fields
};

#endif
