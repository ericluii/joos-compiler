#ifndef __SYMBOLTABLE_H__
#define __SYMBOLTABLE_H__

enum SYMBOL_TABLE_TYPE {
    STT_CLASS;
    STT_INTERFACE;
    STT_FIELD;
    STT_CLASSMETHOD;
    STT_INTERFACEMETHOD;
    STT_CONSTRUCTOR;
    STT_LOCAL;
    STT_NESTEDBLOCK;
    STT_FOR;
};

class SymbolTable {
    protected:
        SymbolTable* prevTable;
        SymbolTable* nextTable;
        TABLE_TYPE tableType;
    public:
        SymbolTable(SYMBOL_TABLE_TYPE type) : prevTable(NULL), nextTable(NULL), tableType(type) {}
        virtual ~SymbolTable() {
            delete prevTable;
            delete nextTable;
        }

        void setPrevTable(SymbolTable* set) { prevTable = set; }
        void setNextTable(SymbolTable* set) { nextTable = set; }
        bool isClassTable() { return tableType == STT_CLASS; }
        bool isInterfaceTable() { return tableType == STT_INTERFACE; }
        bool isFieldTable() { return tableType == STT_FIELD; }
        bool isClassMethodTable() { return tableType == STT_CLASSMETHOD; }
        bool isInterfaceMethodTable() { return tableType == STT_INTERFACEMETHOD; }
        bool isConstructorTable() { return tableType == STT_CONSTRUCTOR; }
        bool isNestedBlockTable() { return tableType == STT_NESTEDBLOCK; }
        bool isForTable() { return tableType == STT_FOR; }

        bool isTopMostEntry() { return prevTable == NULL; }
        bool isBottomMostEntry() { return nextTable == NULL; }
};

#endif
