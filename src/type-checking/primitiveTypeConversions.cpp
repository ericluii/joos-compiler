#include "primitiveTypeConversions.h"

std::set<std::string> PrimitiveTypeConversions::byteWideningConversions = std::set<std::string>({"byte", "short", "int", "long", "float", "double"});
std::set<std::string> PrimitiveTypeConversions::shortWideningConversions = std::set<std::string>({"short", "int", "long", "float", "double"});
std::set<std::string> PrimitiveTypeConversions::charWideningConversions = std::set<std::string>({"char", "int", "long", "float", "double"});
std::set<std::string> PrimitiveTypeConversions::intWideningConversions = std::set<std::string>({"int", "long", "float", "double"});
std::set<std::string> PrimitiveTypeConversions::longWideningConversions = std::set<std::string>({"long", "float", "double"});
std::set<std::string> PrimitiveTypeConversions::floatWideningConversions = std::set<std::string>({"float", "double"});
std::set<std::string> PrimitiveTypeConversions::doubleWideningConversions = std::set<std::string>({"double"});

bool PrimitiveTypeConversions::isWideningConversion(std::string to, std::string from)
{
    if(from == "byte"){
        return byteWideningConversions.count(to);
    }
    if(from == "short"){
        return shortWideningConversions.count(to);
    }
    if(from == "char"){
        return charWideningConversions.count(to);
    }
    if(from == "int"){
        return intWideningConversions.count(to);
    }
    if(from == "long"){
        return longWideningConversions.count(to);
    }
    if(from == "float"){
        return floatWideningConversions.count(to);
    }
    if(from == "double"){
        return doubleWideningConversions.count(to);
    }
    else{
        return false;
    }
}