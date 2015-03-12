#include "package.h"
#include "compilationTable.h"
#include "name.h"

Package::Package(const std::string& name) : packageName(name) {}

Package::~Package() {
    for(std::vector<Package*>::iterator it = subpackages.begin(); it != subpackages.end(); it++) {
        delete *it;
    }
}

std::string& Package::getPkgNameAsString() {
    return packageName;
}

void Package::insertSubpackage(Package* subpackage) {
    subpackages.push_back(subpackage);
}

bool Package::isPackageNameASubpackage(const std::string& subpackage) {
    for(std::vector<Package*>::iterator it = subpackages.begin(); it != subpackages.end(); it++) {
        if((*it)->getPkgNameAsString() == subpackage) {
            return true;
        }
    }
    // couldn't find it as a subpackage
    return false;
}

Package* Package::getSubpackageWithName(const std::string& lookupPkgName) {
    for(std::vector<Package*>::iterator it = subpackages.begin(); it != subpackages.end(); it++) {
        if((*it)->getPkgNameAsString() == lookupPkgName) {
            return *it;
        }
    }
    // couldn't find the subpackage
    return NULL;
}

void Package::registerCompilation(CompilationTable* compilation) {
    // assumption here is that duplicate types have been checked
    compilations.push_back(compilation);
}

CompilationTable* Package::getACompilationWithName(const std::string& lookupName) {
    for(std::vector<CompilationTable*>::iterator it = compilations.begin(); it != compilations.end(); it++) {
        if((*it)->getClassOrInterfaceName() == lookupName) {
            return *it;
        }
    }
    // couldn't find the compilation table
    return NULL;
}
