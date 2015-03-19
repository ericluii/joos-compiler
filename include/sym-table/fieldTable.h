#ifndef __FIELDTABLE_H__
#define __FIELDTABLE_H__

#include "symbolTable.h"

class FieldDecl;
class CompilationTable;

class FieldTable : public SymbolTable {
    private:
        FieldDecl* field;
        // compilation table of the class that declares
        // this field
        CompilationTable* declaringClass;
    public:
        FieldTable(FieldDecl* field, CompilationTable* declaringClass);
        ~FieldTable();
        
        FieldDecl* getField();
        CompilationTable* getDeclaringClass();
        void printSelf(); 
};

#endif
