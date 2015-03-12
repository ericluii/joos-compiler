#include "test_main_common.h"

void cleanUpTokens(std::map<std::string, std::vector<Token*> *>& tokens)
{
    std::map<std::string, std::vector<Token*> *>::iterator it;
    for (it = tokens.begin(); it != tokens.end(); it++) {
        for(unsigned int i = 0; i < (it->second)->size(); i++) {
            delete (it->second)->at(i);
        }
        delete it->second;
    }
}

void cleanUpASTs(std::map<std::string, CompilationUnit*>& ASTs) {
    std::map<std::string, CompilationUnit*>::iterator it;
    for(it = ASTs.begin(); it != ASTs.end(); it++) {
        delete it->second;
    }
}

void cleanUpCompilationTable(std::map<std::string, CompilationTable*>& compilationTables) {
    std::map<std::string, CompilationTable*>::iterator it;
    for(it = compilationTables.begin(); it != compilationTables.end(); it++) {
        delete it->second;
    }
}

void registerPackages(std::map<std::string, std::vector<CompilationTable*> >& packagesCompilations,
                     CompilationTable* table) {
    std::string packageName = table->getPackageName();
    packagesCompilations[packageName].push_back(table);
}

void setOtherCompilations(std::map<std::string, CompilationTable*>& compilationTables,
                          std::map<std::string, std::vector<CompilationTable*> >& packages) {
    std::map<std::string, CompilationTable*>::iterator it;
    for(it = compilationTables.begin(); it != compilationTables.end(); it++) {
        it->second->setCompilationsInPackage(&(packages[it->second->getPackageName()]));
        // check for canonical names here
        it->second->checkForConflictingCanonicalName();
    }
}

void printSymbolTable(SymbolTable* table, int depth) {
    if(table == NULL) return;
    for(int i = 0; i < depth; i++) {
        std::cout << ' ';
    }
    table->printSelf();
    if(table->isClassMethodTable()) {
        printSymbolTable(((ClassMethodTable*)table)->getSymbolTableOfMethod(), depth+1);
    } else if(table->isConstructorTable()) {
        printSymbolTable(((ConstructorTable*)table)->getSymbolTableOfConstructor(), depth+1);
    } else if(table->isNestedBlockTable()) {
        printSymbolTable(((NestedBlockTable*)table)->getSymbolTableOfBlock(), depth+1);
    } else if(table->isForTable()) {
        if(((ForTable*)table)->getForInitTable() != NULL) {
            printSymbolTable(((ForTable*)table)->getForInitTable(), depth+1);
        } else {
            printSymbolTable(((ForTable*)table)->getLoopTable(), depth+1);
        }
    }

    if(!table->isBottomMostEntry()) {
        printSymbolTable(table->getNextTable(), depth);
    }
}

