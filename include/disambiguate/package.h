#ifndef __PACKAGE_H_
#define __PACKAGE_H__

#include <vector>
#include <string>

class CompilationTable;

class Package {
    private:
        std::string packageName;
        std::vector<Package*> subpackages;
        std::vector<CompilationTable*> compilations;
    public:
        Package(const std::string& name);
        ~Package();
        std::string& getPkgNameAsString();
        void insertSubpackage(Package* subpackage);
        bool isPackageNameASubpackage(const std::string& subpackage);
        Package* getSubpackageWithName(const std::string& lookupPkgName);
        void registerCompilation(CompilationTable* compilation);
        CompilationTable* getACompilationWithName(const std::string& lookupName);
};

#endif
