#ifndef __STARTUP_H__
#define __STARTUP_H__

#include <map>
#include <vector>
#include <string>

class CompilationTable;

class Startup {
    private:
        unsigned int typeCounter;
        unsigned int numOfTypes;
        std::map<std::string, CompilationTable*>& compilations;
        std::map<std::string, unsigned int> typeMapping;
        std::map<std::string, std::vector<bool> > inheritanceTable;

        void copyInheritanceTable(const std::string& targetName, const std::string& sourceName);
        void printInheritanceTable();
        void buildInheritanceTable(CompilationTable*);
    public:
        Startup(std::map<std::string, CompilationTable*>& compilations);
        void buildTables();
};

#endif
