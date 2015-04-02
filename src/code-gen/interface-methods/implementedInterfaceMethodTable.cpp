#include "implementedInterfaceMethodTable.h"

// Symbol table
#include "compilationTable.h"
#include "classMethodTable.h"

// AST
#include "classMethod.h"

// Label
#include "labelManager.h"

ImplInterfaceMethodTable::ImplInterfaceMethodTable(const std::string& tableName, ImplInterfaceMethodTable* superclassTable,
            CompilationTable* type, unsigned int numMethods, std::map<std::string, unsigned int>& mapping) :
        tableName(tableName), superclassTable(superclassTable) {
    for(unsigned int i = 0; i < numMethods; i++) {
        implementedMethods.push_back(NULL);
    }

    generateTableForType(type, mapping);
}

void ImplInterfaceMethodTable::generateTableForType(CompilationTable* type, std::map<std::string, unsigned int>& mapping) {
    if(superclassTable != NULL) {
        // copy whatever is in superclass's
        // interface method table
        for(unsigned int i = 0; i < implementedMethods.size(); i++) {
            implementedMethods[i] = superclassTable->implementedMethods[i];
        }
    }

    if(type != NULL) {
        // if type is not NULL
        std::map<std::string, ClassMethodTable*>& classMethods = type->getDefinedClassMethods();
        std::map<std::string, ClassMethodTable*>::iterator it;
        for(it = classMethods.begin(); it != classMethods.end(); it++) {
            if((mapping.count(it->first) == 1)) {
                // if the method signature can be found in the mapping,
                if(!it->second->getClassMethod()->isAbstract()) {
                    // not abstract, register the method
                    implementedMethods[mapping[it->first]] = it->second;
                } else {
                    // is abstract, override whatever was in it with NULL
                    implementedMethods[mapping[it->first]] = NULL;
                }
            }
        }
    }
}

std::string ImplInterfaceMethodTable::generateTableName() {
    return LabelManager::labelizeToInterfaceMethodTable(tableName);
}

void ImplInterfaceMethodTable::outputImplInterfaceMethodTableToFile(std::ofstream& file) {
    std::string outputName = generateTableName();
    file << "global " << outputName << '\n';
    file << outputName << ":\n";
    for(unsigned int i = 0; i < implementedMethods.size(); i++) {
        if(implementedMethods[i] != NULL) {
            std::string methodName = implementedMethods[i]->generateMethodLabel();
            file << "extern " << methodName << '\n';
            file << "  dd " << methodName << '\n';
        } else {
            file << "  dd 0\n";
        }
    }
    file << std::endl;
}
