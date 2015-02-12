#ifndef __NAMEDECL_H__
#define __NAMEDECL_H__

#include "packageDecl.h"
#include "identifier.h"

class Name : public PackageDecl
{
        Name *qualifier;
        Identifier *id;
    public:
        Name(Name *qualifier, Identifier *id);
        Name(Identifier *id); 

        Identifier *GetId();
        Name *GetQualifier();
};

#endif