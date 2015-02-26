#ifndef __FIELDTABLE_H__
#define __FIELDTABLE_H__

#include "symbolTable.h"

class FieldDecl;

class FieldTable : public SymbolTable {
    private:
        FieldDecl* field;
    public:
        FieldTable(FieldDecl* field);
        ~FieldTable();
        
        FieldDecl* getField();
        void printSelf(); 
};

#endif
