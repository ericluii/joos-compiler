#ifndef __INTERFACETABLE_H__
#define __INTERFACETABLE_H__

#include "symbolTable.h"

class InterfaceDecl;

class InterfaceTable : public SymbolTable {
    private:
        InterfaceDecl* declaration;
    public:
        InterfaceTable(InterfaceDecl* declaration) : SymbolTable(STT_INTERFACE), declaration(declaration) {}
        ~InterfaceTable() {}

        InterfaceDecl* getInterface() { return declaration; }
};

#endif
