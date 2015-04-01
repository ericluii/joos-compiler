#ifndef __IMPLINTERFACEMETHODTABLE_H__
#define __IMPLINTERFACEMETHODTABLE_H__

#include <map>
#include <string>
#include <vector>
#include <fstream>

class ClassMethodTable;
class CompilationTable;

class ImplInterfaceMethodTable {
    private:
        std::string tableName;
        ImplInterfaceMethodTable* superclassTable;
        // a vector of class methods that implement
        // some interface method, index of interface method
        // is managed by InterfaceMethodTableManager
        std::vector<ClassMethodTable*> implementedMethods;

        void generateTableForType(CompilationTable*, std::map<std::string, unsigned int>&);
    public:
        ImplInterfaceMethodTable(const std::string&, ImplInterfaceMethodTable*, CompilationTable*,
                             unsigned int, std::map<std::string, unsigned int>&);

        std::string generateTableName();
        void outputImplInterfaceMethodTableToFile(std::ofstream&);
};

#endif
