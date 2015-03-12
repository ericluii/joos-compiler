#ifndef __TEST_MAIN_COMMON__
#define __TEST_MAIN_COMMON__

#include "compilationUnit.h"
#include "buildCompilationTable.h"
#include <map>
#include <vector>

void cleanUpTokens(std::map<std::string, std::vector<Token*> *>& tokens);
void cleanUpASTs(std::map<std::string, CompilationUnit*>& ASTs);
void cleanUpCompilationTable(std::map<std::string, CompilationTable*>& compilationTables);
void registerPackages(std::map<std::string, std::vector<CompilationTable*> >& packagesCompilations,
                      CompilationTable* table);
void setOtherCompilations(std::map<std::string, CompilationTable*>& compilationTables,
                          std::map<std::string, std::vector<CompilationTable*> >& packages);
void printSymbolTable(SymbolTable* table, int depth = 0);

#endif
