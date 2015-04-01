#ifndef __LABELMANAGER_H__
#define __LABELMANAGER_H__

#include <string>

class LabelManager {
    public:
        static std::string labelizeForArrays(const std::string&);
        static std::string labelizeToInheritanceTable(const std::string&);
        static std::string labelizeToVirtualTable(const std::string&);
        static std::string labelizeToInterfaceMethodTable(const std::string&);
};

#endif
