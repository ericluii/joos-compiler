#ifndef __INTERFACEMETHODTABLE_H__
#define __INTERFACEMETHODTABLE_H__

#include <string>
#include "symbolTable.h"

class InterfaceMethod;
class CompilationTable;

class InterfaceMethodTable : public SymbolTable {
    private:
        InterfaceMethod* method;
        // CompilationTable of the interface that
        // declares this method
        CompilationTable* declaringInterface;
    public:
        InterfaceMethodTable(InterfaceMethod* method, CompilationTable* declaringInterface); 
        ~InterfaceMethodTable();

        InterfaceMethod* getInterfaceMethod();
        CompilationTable* getDeclaringInterface();
        void printSelf();
};

#endif
