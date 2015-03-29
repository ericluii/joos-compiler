#ifndef __CODEGENERATOR_H__
#define __CODEGENERATOR_H__

#include <map>
#include <string>

class CompilationTable;
class Startup;
class VTableManager;
class ObjectLayout;

class CodeGenerator {
    private:
        std::map<std::string, CompilationTable*>& compilations;
        CompilationTable* firstUnit;
        Startup* starter;
        VTableManager* virtualManager;
        std::map<CompilationTable*, ObjectLayout*> layoutOfClasses;

        void createObjectLayoutForCompilation(CompilationTable*);
    public:
        CodeGenerator(std::map<std::string, CompilationTable*>&, CompilationTable*);
        ~CodeGenerator();

        void initStage();
};

#endif
