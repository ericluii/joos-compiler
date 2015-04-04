#ifndef __INHERITANCETABLEMANAGER_H__
#define __INHERITANCETABLEMANAGER_H__

#include <map>
#include <string>

class InheritanceTable;
class CompilationTable;

class InheritanceTableManager {
    private:
        unsigned int typeCounter;
        unsigned int numTypes;
        // mapping structure: type name -> their corresponding
        // inheritance table
        std::map<std::string, InheritanceTable*> inheritanceTables;
        // mapping structure: type name -> their corresponding index
        // in the inheritance table
        std::map<std::string, unsigned int> typeMapping;

        void buildInheritanceTableForCompilation(CompilationTable*);
        void buildInheritanceTableForArray(const std::string&, const std::string&);
    public:
        InheritanceTableManager(std::map<std::string, CompilationTable*>&);
        ~InheritanceTableManager();
        
        unsigned int getTypeMapping(const std::string&);
        InheritanceTable* getTableForType(const std::string&);
};

#endif
