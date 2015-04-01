#include "symbolTable.h"

SymbolTable::SymbolTable(SYMBOL_TABLE_TYPE type) : prevTable(NULL), nextTable(NULL), tableType(type) {}

SymbolTable::~SymbolTable() {
    delete nextTable;
}

void SymbolTable::setPrevTable(SymbolTable* set) { prevTable = set; }
void SymbolTable::setNextTable(SymbolTable* set) { nextTable = set; }
SymbolTable* SymbolTable::getPrevTable() { return prevTable; }
SymbolTable* SymbolTable::getNextTable() { return nextTable; }
bool SymbolTable::isClassTable() { return tableType == STT_CLASS; }
bool SymbolTable::isInterfaceTable() { return tableType == STT_INTERFACE; }
bool SymbolTable::isFieldTable() { return tableType == STT_FIELD; }
bool SymbolTable::isClassMethodTable() { return tableType == STT_CLASSMETHOD; }
bool SymbolTable::isInterfaceMethodTable() { return tableType == STT_INTERFACEMETHOD; }
bool SymbolTable::isConstructorTable() { return tableType == STT_CONSTRUCTOR; }
bool SymbolTable::isNestedBlockTable() { return tableType == STT_NESTEDBLOCK; }
bool SymbolTable::isForTable() { return tableType == STT_FOR; }
bool SymbolTable::isLocalTable() { return tableType == STT_LOCAL; }
bool SymbolTable::isParamTable() { return tableType == STT_PARAM; }
SYMBOL_TABLE_TYPE SymbolTable::getTableType() { return tableType; }

bool SymbolTable::isTopMostEntry() { return prevTable == NULL; }
bool SymbolTable::isBottomMostEntry() { return nextTable == NULL; }
