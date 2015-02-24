#ifndef __FORTABLE_H__
#define __FORTABLE_H__

#include "symbolTable.h"

class ForStmt;
class LocalTable;
class NestedBlockTable;

class ForTable : public SymbolTable {
    private:
        ForStmt* stmt;
        LocalTable* forInitTable;
        NestedBlockTable* loopTable;
    public:
        ForTable(ForStmt* stmt) : SymbolTable(STT_FOR), stmt(stmt), forInitTable(NULL), loopTable(NULL) {}
        ~ForTable() {}

        LocalTable* getForInitTable() { return forInitTable; }
        NestedBlockTable* getLoopTable() { return loopTable; }
        void setForInitTable(LocalTable* set) { forInitTable = set; }
        void setLoopTable(NestedBlockTable* set) { loopTable = set; }
};

#endif
