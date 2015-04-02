#include <cassert>
#include <set>

#include "objectLayout.h"
#include "compilationTable.h"
#include "fieldTable.h"
#include "fieldDecl.h"

ObjectLayout::ObjectLayout(ObjectLayout* parentLayout, CompilationTable* table) {
    createLayout(parentLayout, table);
}

void ObjectLayout::createLayout(ObjectLayout* parentLayout, CompilationTable* table) {
    std::set<FieldTable*> registeredFields;
    if(parentLayout != NULL) {
        std::vector<FieldTable*>::iterator it;
        for(it = parentLayout->declaredFields.begin(); it != parentLayout->declaredFields.end(); it++) {
            std::string fieldName = (*it)->getField()->getFieldDeclared()->getIdAsString();
            FieldTable* field = table->getAField(fieldName);
            assert(field != NULL);
            if(!table->fieldIsInherited(fieldName) && !field->getField()->isStatic()) {
                // not inherited and not static
                declaredFields.push_back(*it);
            } else {
                // all other cases
                declaredFields.push_back(field);
                // indicate field has been registered
                registeredFields.insert(field);
            }
        }
    }

    SymbolTable* symTable = table->getSymbolTable()->getNextTable();
    while(symTable != NULL) {
        if(symTable->isFieldTable() && registeredFields.count((FieldTable*) symTable) == 0) {
            // if the symbol table represents a field that has not been registered
            if(!((FieldTable*) symTable)->getField()->isStatic()) {
                // and the field is not static
                declaredFields.push_back((FieldTable*) symTable);
            }
        }
        symTable = symTable->getNextTable();
    }
}

unsigned int ObjectLayout::sizeOfObject() {
    // 12, the size of the virtual table, inheritance table,
    // interface method table pointer,
    // plus the number of fields times 4 bytes (size of each fields)
    return 12 + (declaredFields.size() * 4);
}

unsigned int ObjectLayout::indexOfFieldInObject(FieldTable* field) {
    unsigned int i = 0;
    for(; i < declaredFields.size(); i++) {
        if(declaredFields[i] == field) {
            break;
        }
    }
    return i;
}
