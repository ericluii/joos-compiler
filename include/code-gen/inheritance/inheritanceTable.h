#ifndef __INHERITANCETABLE_H__
#define __INHERITANCETABLE_H__

#include <string>
#include <vector>
#include <fstream>

class InheritanceTable {
    private:
        std::string tableName;
        InheritanceTable* superclassInheritance;
        std::vector<InheritanceTable*> superinterfaceInheritance;

        // a vector of true and false that determines
        // whether a particular type is a subtype of some type
        // index of types is controlled by the
        // InheritanceTableManager
        std::vector<bool> inheritance;

        void copyInheritanceTable(InheritanceTable*);
    public:
        InheritanceTable(const std::string&, InheritanceTable*, unsigned int);
        void pushSuperInterfaceInheritance(InheritanceTable*);

        void generateInheritance(unsigned int);
        std::string generateInheritanceTableName();

        void outputInheritanceTableToFile(std::ofstream&);
};

#endif
