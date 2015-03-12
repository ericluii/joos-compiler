#include <iostream>
#include "paramTable.h"

ParamTable::ParamTable(ParamList* param) : SymbolTable(STT_PARAM), param(param) {}
ParamTable::~ParamTable() {}

ParamList* ParamTable::getParameter() { return param; }
void ParamTable::printSelf() {
    std::cout << "Previous: " << prevTable << "| ParamTable: " << this << "| AST node: " << param 
              << "| Next: " << nextTable << std::endl;
}
