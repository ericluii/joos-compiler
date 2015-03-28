#ifndef __STARTUP_H__
#define __STARTUP_H__

#include <map>
#include <vector>
#include <string>

class CompilationTable;
class ClassMethodTable;
class FieldTable;

class Startup {
    private:
        unsigned int typeCounter;
        unsigned int numOfTypes;
        unsigned int interfaceMethodCounter;
        unsigned int numOfInterfaceMethods;
        std::map<std::string, CompilationTable*>& compilations;
        CompilationTable* firstUnit;

        // inheritance related
        // mapping structure: canonical name -> index in std::vector<bool> of inheritanceTable
        std::map<std::string, unsigned int> typeMapping;
        // mapping structure: type name (canonical name/ canonical name + ".array"/primitive type + ".array")
        // -> a vector of booleans that represent whether the type is a subtype
        // of the type represented at some index i, where i is mapped to the canonical name of some type
        // in typeMapping
        std::map<std::string, std::vector<bool> > inheritanceTable;

        // interface method related
        // mapping structure: method signature -> index in std::vector<ClassMethodTable*> of interfaceMethodTable
        std::map<std::string, unsigned int> interfaceMethodsMapping;
        // mapping structure: method signature -> a vector of the interfaces that declared the method with
        // the particular signature
        std::map<std::string, std::vector<CompilationTable*> > interfaceMethodsDeclaredIn;
        // mapping structure: canonical names (only classes and ".array") -> a vector of the ClassMethodTable* that each
        // represents the class method that implements the interface methods represented at some index i,
        // where i is mapped to an interface method in interfaceMethodsMapping
        std::map<std::string, std::vector<ClassMethodTable*> > interfaceMethodTable;
        // mapping structure: canonical names (only classes) -> a vector of fields defined in the
        // class that are static, order in vector is the same as the order of fields' declaration in class
        std::map<std::string, std::vector<FieldTable*> > staticTable;
        void copyInheritanceTable(const std::string&, const std::string&);
        void copyInterfaceMethodTable(const std::string&, const std::string&);
        void buildInheritanceTable(CompilationTable*);
        void setAllInterfaceMethods();
        void buildInterfaceMethodTable(CompilationTable*);
        void buildStaticTable(CompilationTable*);
        
        void fillInheritanceTableEntriesForArrays(const std::string&);
    public:
        Startup(std::map<std::string, CompilationTable*>&, CompilationTable*);
        void createTablesForCompilation(CompilationTable*);
        void createTablesForArrayType();

        unsigned int getIndexOfInterfaceMethodInTable(const std::string&);
        unsigned int getIndexOfTypeInTable(const std::string&);

        // --------------------------------------------------------------
        // miscellaneous
        void printInheritanceTable();
        void printInterfaceMethodTable();

        // --------------------------------------------------------------
        // file creation
        void generateStartupFile();
};

#endif
