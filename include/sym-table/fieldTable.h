#ifndef __FIELDTABLE_H__
#define __FIELDTABLE_H__

#include "symbolTable.h"

class FieldDecl;

class FieldTable : public SymbolTable {
    private:
        FieldDecl* field;
    public:
        FieldTable(FieldDecl* field) : SymbolTable(STT_FIELD), field(field) {}
        ~FieldTable() {}
        
        FieldDecl* getField() { return field; }
        void printSelf() { 
            std::cout << "FieldTable: " << field << std::endl;
        }
};

#endif
