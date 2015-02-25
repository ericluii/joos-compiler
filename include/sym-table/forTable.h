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
        ForTable(ForStmt* stmt) : SymbolTable(STT_FOR), setTable(false), stmt(stmt), forInitTable(NULL), loopTable(NULL) {}
        ~ForTable() {}

        LocalTable* getForInitTable() { return forInitTable; }
        NestedBlockTable* getLoopTable() { return loopTable; }
        void setForInitTable(LocalTable* set) { forInitTable = set; }
        void setLoopTable(NestedBlockTable* set) { loopTable = set; }
        void printSelf() {
            std::cout << "Previous: " << prevTable << "| ForTable: " << this << "| AST node: " << stmt
                      << "| Next: " << nextTable << std::endl;
        }
        void indicateTableIsSet() { setTable = true; }
        bool isTableSet() { return setTable; }
};

#endif
