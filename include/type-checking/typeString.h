#ifndef __TYPESTRING_H__
#define __TYPESTRING_H__

#include <string>
#include "evaluatedType.h"

extern std::string typeString[];
class CompilationTable;

std::string getCorrespondingTypeString(EVALUATED_TYPE type, CompilationTable* table);

#endif
