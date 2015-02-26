#ifndef __FORTABLE_H__
#define __FORTABLE_H__

#include "symbolTable.h"

class ForStmt;
class LocalTable;
class NestedBlockTable;

class ForTable : public SymbolTable {
    private:
        bool setTable;
        ForStmt* stmt;
        // NULL if forInit is not a local variable declaration or does not exist
        LocalTable* forInitTable;
        // NULL if loop statement does not contain a nested block or empty nested block
        NestedBlockTable* loopTable;
    public:
        ForTable(ForStmt* stmt);
        ~ForTable();

        LocalTable* getForInitTable();
        NestedBlockTable* getLoopTable();
        void setForInitTable(LocalTable* set);
        void setLoopTable(NestedBlockTable* set);
        void printSelf();
        void indicateTableIsSet();
        bool isTableSet();
};

#endif
