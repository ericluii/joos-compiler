#include <iostream>
#include "forTable.h"
#include "localTable.h"
#include "nestedBlockTable.h"

ForTable::ForTable(ForStmt* stmt) : SymbolTable(STT_FOR), setTable(false), stmt(stmt),
                                    forInitTable(NULL), loopTable(NULL) {}

ForTable::~ForTable() {
    if(forInitTable != NULL) {
        forInitTable->setNextTable(NULL);
    }
    delete forInitTable;
    delete loopTable;
}

LocalTable* ForTable::getForInitTable() { return forInitTable; }
SymbolTable* ForTable::getLoopTable() { return loopTable; }
void ForTable::setForInitTable(LocalTable* set) { forInitTable = set; }
void ForTable::setLoopTable(SymbolTable* set) { loopTable = set; }

void ForTable::printSelf() {
    std::cout << "Previous: " << prevTable << "| ForTable: " << this << "| AST node: " << stmt
              << "| Next: " << nextTable << std::endl;
}

void ForTable::indicateTableIsSet() { setTable = true; }
bool ForTable::isTableSet() { return setTable; }

