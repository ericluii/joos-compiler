#ifndef __PRIMITIVE_TYPE_CONVERSIONS_H__
#define __PRIMITIVE_TYPE_CONVERSIONS_H__

#include <string>
#include <set>

class PrimitiveTypeConversions {
    private:
        static std::set<std::string> byteWideningConversions;
        static std::set<std::string> shortWideningConversions;
        static std::set<std::string> charWideningConversions;
        static std::set<std::string> intWideningConversions;
        static std::set<std::string> longWideningConversions;
        static std::set<std::string> floatWideningConversions;
        static std::set<std::string> doubleWideningConversions;
    public:
        static bool isWideningConversion(std::string to, std::string from);

};

#endif
