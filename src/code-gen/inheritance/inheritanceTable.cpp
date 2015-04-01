#include "inheritanceTable.h"
#include "labelManager.h"

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
