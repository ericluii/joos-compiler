#ifndef __PARAMTABLE_H__
#define __PARAMTABLE_H__

#include "symbolTable.h"

class ParamList;

class ParamTable : public SymbolTable {
    private:
        ParamList* param;
    public:
        ParamTable(ParamList* param);
        ~ParamTable();

        ParamList* getParameter();
        void printSelf();
};

#endif
