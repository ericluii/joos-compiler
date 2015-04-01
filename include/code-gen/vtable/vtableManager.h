#ifndef __VTABLEMANAGER_H__
#define __VTABLEMANAGER_H__

#include <map>
#include <string>

class CompilationTable;
class VTableLayout;

class VTableManager {
    private:
        std::map<std::string, CompilationTable*>& compilations;
        // mapping structure: type names (canonical names/.array) -> respective VTableLayout
        std::map<std::string, VTableLayout*> vTableCollection;
    public:
        VTableManager(std::map<std::string, CompilationTable*>&);
        ~VTableManager();
        void createVTableLayoutForCompilation(CompilationTable*);
        void createVTableLayoutForArrays();
        VTableLayout* getVTableLayoutForType(const std::string&);
        VTableLayout* getVTableLayoutForArray();
};

#endif
