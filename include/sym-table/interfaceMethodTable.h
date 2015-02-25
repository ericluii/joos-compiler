#ifndef __INTERFACEMETHODTABLE_H__
#define __INTERFACEMETHODTABLE_H__

#include "symbolTable.h"

class InterfaceMethod;

class InterfaceMethodTable : public SymbolTable {
    private:
        InterfaceMethod* method;
    public:
        InterfaceMethodTable(InterfaceMethod* method) : SymbolTable(STT_INTERFACEMETHOD), method(method) {}
        ~InterfaceMethodTable() {}

        InterfaceMethod* getInterfaceMethod() { return method; }
        void printSelf() {
            std::cout << "Previous: " << prevTable << "| InterfaceMethodTable: " << this << "| AST node: " << method 
                      << "| Next: " << nextTable << std::endl;
        }
};

#endif
