#ifndef __LOCALTABLE_H__
#define __LOCALTABLE_H__

#include "symbolTable.h"

class LocalDecl;

class LocalTable : public SymbolTable {
    private:
        LocalDecl* local;
    public:
        LocalTable(LocalDecl* local); 
        ~LocalTable();

        LocalDecl* getLocalDecl();
        void printSelf(); 
};

#endif
