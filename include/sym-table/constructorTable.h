#ifndef __CONSTRUCTORTABLE_H__
#define __CONSTRUCTORTABLE_H__

#include "symbolTable.h"

class Constructor;

class ConstructorTable : public SymbolTable {
    private:
        Constructor* constructor;
        SymbolTable* bodyTable;
    public:
        ConstructorTable(Constructor* constructor); 
        ~ConstructorTable(); 

        Constructor* getConstructor(); 
        SymbolTable* getSymbolTableOfConstructor(); 
        void setSymTableOfConstructor(SymbolTable* set); 

        void printSelf(); 
};

#endif
