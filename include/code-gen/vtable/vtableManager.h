#ifndef __VTABLEMANAGER_H__
#define __VTABLEMANAGER_H__

#include <map>
#include <string>

class CompilationTable;
class VTableLayout;

class VTableManager {
    private:
        std::map<std::string, CompilationTable*>& compilations;
        std::map<std::string, VTableLayout*> vTableCollection;
    public:
        VTableManager(std::map<std::string, CompilationTable*>&);
        ~VTableManager();
        void createVTableLayoutForCompilation(CompilationTable*);
        void createVTableLayoutForArrays();
        VTableLayout* getVTableLayoutOfType(const std::string&);
};

#endif
