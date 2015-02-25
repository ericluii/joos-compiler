#ifndef __COMPILATIONTABLE_H__
#define __COMPILATIONTABLE_H__

#include <string>
#include "packageDecl.h"
#include "symbolTable.h"

class CompilationTable {
    private:
        PackageDecl* package;
        SymbolTable* symTable;
    public:
        CompilationTable(PackageDecl* package) : package(package), symTable(NULL) {}
        ~CompilationTable() {
            delete symTable;
        }

        SymbolTable* getSymbolTable() { return symTable; }
        std::string getPackageName() { return package->getPackageName()->getFullName(); }
        void setSymbolTable(SymbolTable* set) { symTable = set; }
};

#endif
