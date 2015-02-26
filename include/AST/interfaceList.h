#ifndef __INTERFACELIST_H__
#define __INTERFACELIST_H__

#include "ast.h"
#include "interfaces.h"

class InterfaceList : public Ast {
    // Rule: IMPLEMENTING, IMPLEMENT_EPSILON, EXTENDS_INTERFACE and EXTENDS_INTERFACE_EPSILON
    private:
        Interfaces* list;
    public:
        InterfaceList(Interfaces* list) : list(list) {}
        ~InterfaceList() {}

        Interfaces* getListOfInterfaces() { return list; }
        bool isEpsilon() { return list == NULL; }
};

#endif
