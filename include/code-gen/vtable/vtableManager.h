#ifndef __VTABLEMANAGER_H__
#define __VTABLEMANAGER_H__

#include <map>
#include <string>

class CompilationTable;
class VTableLayout;

class VTableManager {
    private:
        // mapping structure: type names (canonical names, only classes and arrays)
        //                    -> respective VTableLayout
        std::map<std::string, VTableLayout*> vTableCollection;

        void createVTableLayoutForCompilation(CompilationTable*);
        void createVTableLayoutForArrays();
    public:
        VTableManager(std::map<std::string, CompilationTable*>&);
        ~VTableManager();
        VTableLayout* getVTableLayoutForType(const std::string&);
        VTableLayout* getVTableLayoutForArray();
};

#endif
