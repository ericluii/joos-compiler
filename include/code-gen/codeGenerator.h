#ifndef __CODEGENERATOR_H__
#define __CODEGENERATOR_H__

#include <map>
#include <string>

class CompilationTable;
class Startup;
class VTableManager;

class CodeGenerator {
    private:
        std::map<std::string, CompilationTable*>& compilations;
        Startup* starter;
        VTableManager* virtualManager;
    public:
        CodeGenerator(std::map<std::string, CompilationTable*>&);
        ~CodeGenerator();

        void initStage();
};

#endif
