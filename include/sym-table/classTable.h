#ifndef __CLASSTABLE_H__
#define __CLASSTABLE_H__

#include "symbolTable.h"

class ClassDecl;

class ClassTable : public SymbolTable {
    private:
        ClassDecl* declaration;
    public:
        ClassTable(ClassDecl* declaration) : SymbolTable(STT_CLASS), declaration(declaration) {}
        ~ClassTable() {}

        ClassDecl* getClass() { return declaration; }
        void printSelf() {
            std::cout << "ClassTable: " << this << "| AST node: " << declaration
                      << "| Next: " << nextTable << std::endl;
        }
};

#endif
