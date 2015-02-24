#ifndef __INTERFACEMETHODTABLE_H__
#define __INTERFACEMETHODTABLE_H__

#include "symbolTable.h"

class InterfaceMethod;

class InterfaceMethodTable : public SymbolTable {
    private:
        InterfaceMethod* method;
    public:
        InterfaceMethodTable(InterfaceMethod* method) : SymbolTable(STT_INTERFACEMETHOD), method(method) {}
        ~InterfaceMethod() {}

        InterfaceMethod* getInterfaceMethod() { return method; }
};

#endif
