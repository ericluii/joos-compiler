#ifndef __SYMBOLTABLE_H__
#define __SYMBOLTABLE_H__

#include <string>
#include <iostream>

enum SYMBOL_TABLE_TYPE {
    STT_CLASS = 0,
    STT_INTERFACE,
    STT_FIELD,
    STT_CLASSMETHOD,
    STT_INTERFACEMETHOD,
    STT_CONSTRUCTOR,
    STT_LOCAL,
    STT_NESTEDBLOCK,
    STT_FOR,
};

class SymbolTable {
    protected:
        SymbolTable* prevTable;
        SymbolTable* nextTable;
        SYMBOL_TABLE_TYPE tableType;
    public:
        SymbolTable(SYMBOL_TABLE_TYPE type) : prevTable(NULL), nextTable(NULL), tableType(type) {}
        virtual ~SymbolTable() {
            delete nextTable;
        }

        void setPrevTable(SymbolTable* set) { prevTable = set; }
        void setNextTable(SymbolTable* set) { nextTable = set; }
        SymbolTable* getPrevTable() { return prevTable; }
        SymbolTable* getNextTable() { return nextTable; }
        bool isClassTable() { return tableType == STT_CLASS; }
        bool isInterfaceTable() { return tableType == STT_INTERFACE; }
        bool isFieldTable() { return tableType == STT_FIELD; }
        bool isClassMethodTable() { return tableType == STT_CLASSMETHOD; }
        bool isInterfaceMethodTable() { return tableType == STT_INTERFACEMETHOD; }
        bool isConstructorTable() { return tableType == STT_CONSTRUCTOR; }
        bool isNestedBlockTable() { return tableType == STT_NESTEDBLOCK; }
        bool isForTable() { return tableType == STT_FOR; }
        bool isLocalTable() { return tableType == STT_LOCAL; }
        SYMBOL_TABLE_TYPE getTableType() { return tableType; }

        bool isTopMostEntry() { return prevTable == NULL; }
        bool isBottomMostEntry() { return nextTable == NULL; }

        void virtual printSelf() = 0;
};

#endif
