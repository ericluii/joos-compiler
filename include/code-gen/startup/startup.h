#ifndef __STARTUP_H__
#define __STARTUP_H__

#include <map>
#include <vector>
#include <string>

class CompilationTable;
class VTableLayout;
class InheritanceTable;
class ImplInterfaceMethodTable;
class StaticFields;

class Startup {
    private:
        std::map<std::string, CompilationTable*>& compilations;
        CompilationTable* firstUnit;
    public:
        Startup(std::map<std::string, CompilationTable*>&, CompilationTable*);

        // --------------------------------------------------------------
        // file creation
        void generateStartupFile(VTableLayout*, std::vector<InheritanceTable*>&,
                                 ImplInterfaceMethodTable*, std::vector<StaticFields*>&);
};

#endif
