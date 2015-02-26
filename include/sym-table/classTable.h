#ifndef __CLASSTABLE_H__
#define __CLASSTABLE_H__

#include "symbolTable.h"

class ClassDecl;

class ClassTable : public SymbolTable {
    private:
        ClassDecl* declaration;
    public:
        ClassTable(ClassDecl* declaration);
        ~ClassTable();

        ClassDecl* getClass();
        void printSelf();
};

#endif
