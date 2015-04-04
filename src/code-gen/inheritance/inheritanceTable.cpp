#include "inheritanceTable.h"
#include "labelManager.h"

#include<iostream>
InheritanceTable::InheritanceTable(const std::string& tableName, InheritanceTable* superclassInheritance, unsigned int numTypes) :
        tableName(tableName), superclassInheritance(superclassInheritance) {
    for(unsigned int i = 0; i < numTypes; i++) {
        inheritance.push_back(false);
    }
}

void InheritanceTable::pushSuperInterfaceInheritance(InheritanceTable* inhTable) {
    superinterfaceInheritance.push_back(inhTable);
}

void InheritanceTable::copyInheritanceTable(InheritanceTable* inhTable) {
    for(unsigned int i = 0; i < inhTable->inheritance.size(); i++) {
        if(inhTable->inheritance[i]) {
            // if at some particular index the inheritance
            // table says that the type is a subtype, then make
            // the current type a subtype of the type at that
            // particular index too
            inheritance[i] = inhTable->inheritance[i];
        }
    }
}

void InheritanceTable::generateInheritance(unsigned int selfIndex) {
    if(superclassInheritance != NULL) {
        copyInheritanceTable(superclassInheritance);
    }

    for(unsigned int i = 0; i < superinterfaceInheritance.size(); i++) {
        copyInheritanceTable(superinterfaceInheritance[i]);
    }

    // indicate that type is subtype of
    // itself
    inheritance[selfIndex] = true;
}

std::string InheritanceTable::generateInheritanceTableName() {
    return LabelManager::labelizeToInheritanceTable(tableName);
}

void InheritanceTable::outputInheritanceTableToFile(std::ofstream& file) {
    std::string outputName = generateInheritanceTableName();
    file << "global " << outputName << '\n';
    file << outputName << ":\n";
    for(unsigned int i = 0 ; i < inheritance.size(); i++) {
        if(i == 0) { file << "  db "; }
        else { file << ','; }
        file << inheritance[i];
    }
    file << '\n' << std::endl;
}

bool InheritanceTable::inheritanceStatusForType(unsigned int mappingOfType) {
    return inheritance[mappingOfType];
}

void InheritanceTable::setInheritanceForAType(unsigned int mappingOfType, bool inheritanceStatus) {
    inheritance[mappingOfType] = inheritanceStatus;
}
