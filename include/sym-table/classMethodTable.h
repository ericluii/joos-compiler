#ifndef __CLASSMETHODTABLE_H__
#define __CLASSMETHODTABLE_H__

#include "symbolTable.h"

class ClassMethod;
class CompilationTable;

class ClassMethodTable : public SymbolTable {
    private:
        ClassMethod* method;
        SymbolTable* bodyTable;
        // compilation table of the class that declares
        // this method
        CompilationTable* declaringClass;
        bool registeredVTable;
    public:
        ClassMethodTable(ClassMethod* method, CompilationTable* declaringClass); 
        ~ClassMethodTable(); 

        ClassMethod* getClassMethod();
        SymbolTable* getSymbolTableOfMethod();
        CompilationTable* getDeclaringClass();
        void setSymTableOfMethod(SymbolTable* set);

        void printSelf();
};

#endif
