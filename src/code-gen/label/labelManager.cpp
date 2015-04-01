#include "labelManager.h"

std::string LabelManager::labelizeForArrays(const std::string& toLabelize) {
    return toLabelize + "$array";
}

std::string LabelManager::labelizeToInheritanceTable(const std::string& toLabelize) {
    return "INH$" + toLabelize;
}

std::string LabelManager::labelizeToVirtualTable(const std::string& toLabelize) {
    return "VIRT$" + toLabelize;
}

std::string LabelManager::labelizeToInterfaceMethodTable(const std::string& toLabelize) {
    return "INTER$" + toLabelize;
}
