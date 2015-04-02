#ifndef __LABELMANAGER_H__
#define __LABELMANAGER_H__

#include <string>

class LabelManager {
    private:
        static std::string labelizer;
        static std::string virt;
        static std::string inheritance;
        static std::string implInterface;
        static std::string initializer;
        static std::string array;
    public:
        static std::string labelizeForArrays(const std::string&);
        static std::string labelizeToInheritanceTable(const std::string&);
        static std::string labelizeToVirtualTable(const std::string&);
        static std::string labelizeToInterfaceMethodTable(const std::string&);
        static std::string labelizeToStaticInitializer(const std::string&);
        static std::string getLabelForArrayVirtualTable();
        static std::string getLabelForArrayImplInterfaceMethodTable();
        static std::string getLabelForArrayInheritanceTable(const std::string&);
};

#endif
