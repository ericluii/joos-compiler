#ifndef __INTERFACELIST_H__
#define __INTERFACELIST_H__

#include "ast.h"
#include "implements.h"

class InterfaceList : public Ast {
    private:
        Implements* implementsList;
    public:
        InterfaceList(Implements* implementsList) : implementsList(implementsList) {}

        Implements* getImplementedInterfaces() { return implementsList; }
        bool isEpsilon() { return implementsList == NULL; }
};

#endif
