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
            if(table->fieldIsInherited(fieldName)) {
                // just copy everything of the parent's
                declaredFields.push_back(*it);
                staticFieldsIndicator[*it] = parentLayout->staticFieldsIndicator[*it];
            } else {
                FieldTable* ownField = table->getAField(fieldName);
                // precautionary check
                assert(ownField != NULL);
                declaredFields.push_back(ownField);
                // indicate it's a static field or not
                if(ownField->getField()->isStatic()) {
                    staticFieldsIndicator[ownField] = true;
                } else {
                    staticFieldsIndicator[ownField] = false;
                }
                // indicate field has been registered
                registeredFields.insert(ownField);
            }
        }
    }

    SymbolTable* symTable = table->getSymbolTable()->getNextTable();
    while(symTable != NULL) {
        if(symTable->isFieldTable() && registeredFields.count((FieldTable*) symTable) == 0) {
            declaredFields.push_back((FieldTable*) symTable);
            if(((FieldTable*) symTable)->getField()->isStatic()) {
                staticFieldsIndicator[(FieldTable*) symTable] = true;
            } else {
                staticFieldsIndicator[(FieldTable*) symTable] = false;
            }
        }
        symTable = symTable->getNextTable();
    }
}

unsigned int ObjectLayout::sizeOfObject() {
    // 16, the size of the virtual table, inheritance table,
    // interface method table pointer and static indicator table,
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

void ObjectLayout::generateStaticIndicatorRowToFile(std::ofstream& fs) {
    for(unsigned int i = 0; i < declaredFields.size(); i++) {
        if(i == 0) {
            fs << "dd ";
        } else {
            fs << ",";
        }
        fs << staticFieldsIndicator[declaredFields[i]];
    }
    fs << '\n';
}
