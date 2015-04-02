#include "labelManager.h"

std::string LabelManager::labelizer = "$";
std::string LabelManager::virt = "VIRT" + LabelManager::labelizer;
std::string LabelManager::inheritance = "INH" + LabelManager::labelizer;
std::string LabelManager::implInterface = "INTER" + LabelManager::labelizer;
std::string LabelManager::initializer = "INIT" + LabelManager::labelizer;
std::string LabelManager::array = ".array";

std::string LabelManager::labelizeForArrays(const std::string& toLabelize) {
    return toLabelize + array;
}

std::string LabelManager::labelizeToInheritanceTable(const std::string& toLabelize) {
    return LabelManager::inheritance + toLabelize;
}

std::string LabelManager::labelizeToVirtualTable(const std::string& toLabelize) {
    return LabelManager::virt + toLabelize;
}

std::string LabelManager::labelizeToInterfaceMethodTable(const std::string& toLabelize) {
    return LabelManager::implInterface + toLabelize;
}

std::string LabelManager::labelizeToStaticInitializer(const std::string& toLabelize) {
    return LabelManager::initializer + toLabelize;
}

std::string LabelManager::getLabelForArrayImplInterfaceMethodTable() {
    return LabelManager::implInterface + array;
}

std::string LabelManager::getLabelForArrayVirtualTable() {
    return LabelManager::virt + LabelManager::array;
}

std::string LabelManager::getLabelForArrayInheritanceTable(const std::string& arrayComponentType) {
    return LabelManager::labelizeForArrays(LabelManager::labelizeToInheritanceTable(arrayComponentType));
}
