#ifndef __COMPILATIONTABLE_H__
#define __COMPILATIONTABLE_H__

#include <string>
#include "packageDecl.h"
#include "symbolTable.h"

class CompilationTable {
    private:
        PackageDecl* package;
        SymbolTable* typeTable;
    public:
        CompilationTable(PackageDecl* package) : package(package), typeTable(NULL) {}
        ~CompilationTable() {
            delete typeTable;
        }

        Symboltable* getTableOfType() { return typeTable; }
        std::string getPackageName() { return package->getPackageName()->getFullName(); }
}

#endif
