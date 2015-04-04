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
        static std::string constructor;
        static std::string alloc;
        static std::string native;
    public:
        static std::string getLabelizer();
        static std::string getConstructor();
        static std::string labelizeForAlloc(const std::string& toLabelize);
        static std::string labelizeForConstructor(const std::string& toLabelize);
        static std::string labelizeForConstructor(const std::string&, int n_args, ...);
        static std::string labelizeForArrays(const std::string&);
        static std::string labelizeToInheritanceTable(const std::string&);
        static std::string labelizeToVirtualTable(const std::string&);
        static std::string labelizeToInterfaceMethodTable(const std::string&);
        static std::string labelizeToFieldInitializer(const std::string&);
        static std::string getLabelForArrayVirtualTable();
        static std::string getLabelForArrayImplInterfaceMethodTable();
        static std::string getLabelForArrayInheritanceTable(const std::string&);
        static std::string labelizeToNativeMethod(const std::string&);
};

#endif
