#ifndef __STATICFIELDSMANAGER_H__
#define __STATICFIELDSMANAGER_H__

#include <map>
#include <string>

class CompilationTable;
class StaticFields;

class StaticFieldsManager {
    private:
        std::map<CompilationTable*, StaticFields*> classStatics;
    public:
        StaticFieldsManager(std::map<std::string, CompilationTable*>&);
        ~StaticFieldsManager();

        StaticFields* getStaticFieldsForClass(CompilationTable*);
};

#endif
