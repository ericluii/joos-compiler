#include <cassert>
#include <set>

#include "objectLayout.h"
#include "compilationTable.h"
#include "fieldTable.h"
#include "fieldDecl.h"

// there are three tables, each table is 4 bytes
unsigned int ObjectLayout::sizeOfTables = 12;

ObjectLayout::ObjectLayout(ObjectLayout* parentLayout, CompilationTable* table) : parentLayout(parentLayout) {
    createLayout(parentLayout, table);
}

void ObjectLayout::createLayout(ObjectLayout* parentLayout, CompilationTable* table) {
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
    unsigned int sizeOfParent = 0;
    if(parentLayout != NULL) {
        // get the size of the parent
        sizeOfParent = sizeOfObject();
    } else {
        // no superclass, must be java.lang.Object
        // return the size of the tables + the number of
        // fields defined multiplied by 4
        return ObjectLayout::sizeOfTables + (declaredFields.size() * 4);
    }

    // return size of parent + number of declared fields multiplied by 4
    return sizeOfParent + declaredFields.size() * 4;
}

unsigned int ObjectLayout::indexOfFieldInObject(FieldTable* field) {
    for(unsigned int i = 0; i < declaredFields.size(); i++) {
        if(declaredFields[i] == field) {
            // multiplied by 4 for doubleword access (32 bits)
            // plus the size of the tables
            return i * 4 + ObjectLayout::sizeOfTables;
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

unsigned int ObjectLayout::transformToFieldIndexInAClass(unsigned int i) {
    return i * 4 + ObjectLayout::sizeOfTables;
}
