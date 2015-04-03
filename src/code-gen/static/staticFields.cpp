#include "staticFields.h"

// Symbol table
#include "compilationTable.h"
#include "symbolTable.h"
#include "fieldTable.h"

// AST
#include "fieldDecl.h"

StaticFields::StaticFields(CompilationTable* table) {
    SymbolTable* symTable = table->getSymbolTable()->getNextTable();
    while(symTable != NULL) {
        if(symTable->isFieldTable() && ((FieldTable*) symTable)->getField()->isStatic()
           && ((FieldTable*) symTable)->getField()->isInitialized()) {
            // it is a static field that has an initializer
            statics.push_back((FieldTable*) symTable);
        }
        symTable = symTable->getNextTable();
    }
}

std::vector<FieldTable*>& StaticFields::getAllStaticFieldsOfClass() {
    return statics;
}
