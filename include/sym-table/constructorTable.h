#ifndef __CONSTRUCTORTABLE_H__
#define __CONSTRUCTORTABLE_H__

#include "symbolTable.h"

class Constructor;
class CompilationTable;

class ConstructorTable : public SymbolTable {
    private:
        Constructor* constructor;
        SymbolTable* bodyTable;
        // CompilationTable of the class that declares
        // this constructor
        CompilationTable* declaringClass;
    public:
        ConstructorTable(Constructor* constructor, CompilationTable* declaringClass); 
        ~ConstructorTable(); 

        Constructor* getConstructor(); 
        SymbolTable* getSymbolTableOfConstructor();
        CompilationTable* getDeclaringClass();
        void setSymTableOfConstructor(SymbolTable* set); 

        void printSelf(); 
};

#endif
