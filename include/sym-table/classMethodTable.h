#ifndef __CLASSMETHODTABLE_H__
#define __CLASSMETHODTABLE_H__

#include "symbolTable.h"

class ClassMethod;

class ClassMethodTable : public SymbolTable {
    private:
        ClassMethod* method;
        SymbolTable* bodyTable;
    public:
        ClassMethodTable(ClassMethod* method); 
        ~ClassMethodTable(); 

        ClassMethod* getClassMethod();
        SymbolTable* getSymbolTableOfMethod();
        void setSymTableOfMethod(SymbolTable* set);

        void printSelf();
};

#endif
