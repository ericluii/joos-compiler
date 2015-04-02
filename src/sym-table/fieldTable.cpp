#include <iostream>
#include "fieldTable.h"

#include "fieldDecl.h"
#include "compilationTable.h"

#include "labelManager.h"

FieldTable::FieldTable(FieldDecl* field, CompilationTable* declaringClass) : SymbolTable(STT_FIELD), field(field), 
            declaringClass(declaringClass) {}
FieldTable::~FieldTable() {}

FieldDecl* FieldTable::getField() { return field; }

CompilationTable* FieldTable::getDeclaringClass() { return declaringClass; }

void FieldTable::printSelf() {
    std::cout << "Previous: " << prevTable << "| FieldTable: " << this << "| AST node: " << field 
              << "| Next: " << nextTable << std::endl;
}

std::string FieldTable::generateFieldLabel() {
    return declaringClass->getCanonicalName() + '.' +
           field->getFieldDeclared()->getIdAsString();
}

std::string FieldTable::generateStaticInitializerLabel() {
    return LabelManager::labelizeToStaticInitializer(generateFieldLabel());
}
