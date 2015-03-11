#include <cassert>
#include "typeString.h"
#include "compilationTable.h"

// the order here resembles the order defined
// in evaluatedType.h, each type there corresponds
// to a string here
std::string typeString [] = {
    "int",
    "short",
    "byte",
    "char",
    "boolean",
    "int[]",
    "short[]",
    "byte[]",
    "char[]",
    "boolean[]",
    // yes there is a 'type' null,
    // at least for our purposes lol
    "null",
    // again for our purposes, there's void
    "void",
    // N/A really refers to expression types
    // which failed to be evaluated, due to some
    // error
    "N/A"
    // the rest of course are objects and need to be
    // looked up manually
};

std::string getCorrespondingTypeString(EVALUATED_TYPE type, CompilationTable* table) {
    std::string ret;
    if(type != ET_OBJECT && type != ET_OBJECTARRAY) {
        ret = typeString[type];
    } else {
        // check that an actual compilation table has been
        // given
        assert(table != NULL);
        ret = table->getCanonicalName();
    }
    return ret;
}
