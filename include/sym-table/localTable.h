#ifndef __LOCALTABLE_H__
#define __LOCALTABLE_H__

#include "symbolTable.h"

class LocalDecl;

class LocalTable : public SymbolTable {
    private:
        LocalDecl* local;
    public:
        LocalTable(LocalDel* local) : SymbolTable(STT_LOCAL), local(local) {}
        ~LocalTable() {}

        LocalDel* getLocalDecl() { return local; }
};

#endif
