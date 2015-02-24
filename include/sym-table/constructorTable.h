#ifndef __CONSTRUCTORTABLE_H__
#define __CONSTRUCTORTABLE_H__

#include "symbolTable.h"

class Constructor;

class ConstructorTable : public SymbolTable {
    private:
        Constructor* constructor;
    public:
        ConstructorTable(Constructor* constructor) : SymbolTable(STT_CONSTRUCTOR), constructor(constructor) {}
        ~ConstructorTable() {}

        Constructor* getConstructor() { return constructor; }
};

#endif
