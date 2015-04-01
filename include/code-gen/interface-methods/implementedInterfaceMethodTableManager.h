#ifndef __IMPLINTERFACEMETHODTABLEMANAGER_H__
#define __IMPLINTERFACEMETHODTABLEMANAGER_H__

#include <map>
#include <string>

class ImplInterfaceMethodTable;
class CompilationTable;

class ImplInterfaceMethodTableManager {
    private:
        unsigned int methodCounter;
        unsigned int numMethods;
        // mapping structure: type names/.array -> corresponding
        // interface method tables
        std::map<std::string, ImplInterfaceMethodTable*> tables;

        // mapping structure: method signature -> an index that
        // represents the method with that particular method signature
        // in the interface method table
        std::map<std::string, unsigned int> methodMapping;

        void initializeMapping(std::map<std::string, CompilationTable*>&);
        void generateTableForClass(CompilationTable*);
        void generateTableForArray();
    public:
        ImplInterfaceMethodTableManager(std::map<std::string, CompilationTable*>&);
        ~ImplInterfaceMethodTableManager();
        
        unsigned int getInterfaceMethodMapping(const std::string&);
        ImplInterfaceMethodTable* getTableForType(const std::string&);
        ImplInterfaceMethodTable* getTableForArray();
};

#endif
