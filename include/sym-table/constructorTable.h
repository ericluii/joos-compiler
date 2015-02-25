#ifndef __CONSTRUCTORTABLE_H__
#define __CONSTRUCTORTABLE_H__

#include "symbolTable.h"

class Constructor;

class ConstructorTable : public SymbolTable {
    private:
        Constructor* constructor;
        SymbolTable* bodyTable;
    public:
        ConstructorTable(Constructor* constructor) : SymbolTable(STT_CONSTRUCTOR), constructor(constructor) {}
        ~ConstructorTable() {}

        Constructor* getConstructor() { return constructor; }
        SymbolTable* getSymbolTableOfConstructor() { return bodyTable; } 
        void setSymTableOfConstructor(SymbolTable* set) { bodyTable = set; }

        void printSelf() { 
            std::cout << "ConstructorTable: " << constructor << std::endl;
        }
};

#endif
