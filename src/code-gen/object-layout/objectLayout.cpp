#include <cassert>
#include <set>

#include "objectLayout.h"
#include "compilationTable.h"
#include "fieldTable.h"
#include "fieldDecl.h"

// there are three tables, each table is 4 bytes
// plus a type number which is another 4 bytes
unsigned int ObjectLayout::sizeOfTables = 16;

ObjectLayout::ObjectLayout(ObjectLayout* parentLayout, CompilationTable* table) : parentLayout(parentLayout) {
    createLayout(table);
}

void ObjectLayout::createLayout(CompilationTable* table) {
    SymbolTable* symTable = table->getSymbolTable()->getNextTable();
    while(symTable != NULL) {
        if(symTable->isFieldTable()) {
            // if the symbol table represents a field
            if(!((FieldTable*) symTable)->getField()->isStatic()) {
                // and the field is not static
                declaredFields.push_back((FieldTable*) symTable);
            }
        }
        symTable = symTable->getNextTable();
    }
}

unsigned int ObjectLayout::sizeOfObject() {
    if(parentLayout != NULL) {
        // return size of parent + number of declared fields multiplied by 4
        return parentLayout->sizeOfObject() + declaredFields.size() * 4;
    } else {
        // no superclass, must be java.lang.Object
        // return the size of the tables + the number of
        // fields defined multiplied by 4
        return ObjectLayout::sizeOfTables + (declaredFields.size() * 4);
    }
}

unsigned int ObjectLayout::indexOfFieldInObject(FieldTable* field) {
    for(unsigned int i = 0; i < declaredFields.size(); i++) {
        if(declaredFields[i] == field) {
            // multiplied by 4 for doubleword access (32 bits)
            // plus the size of the tables
            if(parentLayout != NULL) {
                // also add in the number of fields in the parent
                // which are also in this class
                return (i + parentLayout->numberOfFieldsInObject()) * 4 + ObjectLayout::sizeOfTables;
            } else {
                return i * 4 + ObjectLayout::sizeOfTables;
            }
        }
    }

    // return whatever is in the parent, assumption
    // is that we can always find it, or else things will break
    if(parentLayout != NULL) {
        return parentLayout->indexOfFieldInObject(field);
    }

    // if we got here then we must have reached
    // java.lang.Object and couldn't find the field
    assert(false);
}

unsigned int ObjectLayout::numberOfFieldsInObject() {
    // gets the number of fields in a class, including hidden ones
    if(parentLayout != NULL) {
        return parentLayout->numberOfFieldsInObject() + declaredFields.size();
    } else {
        return declaredFields.size();
    }
}

unsigned int ObjectLayout::transformToFieldIndexInAnObject(unsigned int i) {
    return i * 4 + ObjectLayout::sizeOfTables;
}
