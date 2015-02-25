#ifndef __CLASSMETHODTABLE_H__
#define __CLASSMETHODTABLE_H__

#include "symbolTable.h"

class ClassMethod;

class ClassMethodTable : public SymbolTable {
    private:
        ClassMethod* method;
        SymbolTable* bodyTable;
    public:
        ClassMethodTable(ClassMethod* method) : SymbolTable(STT_CLASSMETHOD), method(method), bodyTable(NULL) {}
        ~ClassMethodTable() {}

        ClassMethod* getClassMethod() { return method; }
        SymbolTable* getSymbolTableOfMethod() { return bodyTable; }
        void setSymTableOfMethod(SymbolTable* set) { bodyTable = set; }

        void printSelf() { 
            std::cout << "ClassMethodTable: " << method << std::endl;
        }
};

#endif
