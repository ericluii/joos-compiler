#ifndef __SYMBOLTABLE_H__
#define __SYMBOLTABLE_H__

enum SYMBOL_TABLE_TYPE {
    STT_CLASS = 0,
    STT_INTERFACE,
    STT_FIELD,
    STT_CLASSMETHOD,
    STT_INTERFACEMETHOD,
    STT_CONSTRUCTOR,
    STT_PARAM,
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
        SymbolTable(SYMBOL_TABLE_TYPE type);
        virtual ~SymbolTable();

        void setPrevTable(SymbolTable* set);
        void setNextTable(SymbolTable* set);
        SymbolTable* getPrevTable();
        SymbolTable* getNextTable();
        bool isClassTable();
        bool isInterfaceTable();
        bool isFieldTable();
        bool isClassMethodTable(); 
        bool isInterfaceMethodTable();
        bool isConstructorTable();
        bool isNestedBlockTable(); 
        bool isForTable(); 
        bool isLocalTable();
        bool isParamTable();
        SYMBOL_TABLE_TYPE getTableType(); 

        bool isTopMostEntry();
        bool isBottomMostEntry();

        void virtual printSelf() = 0;
};

#endif
