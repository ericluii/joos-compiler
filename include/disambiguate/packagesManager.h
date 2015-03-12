#ifndef __PACKAGESMANAGER_H__
#define __PACKAGESMANAGER_H__

#include <vector>
#include <map>
#include <string>

class Package;
class Name;
class CompilationTable;

class PackagesManager {
    private:
        bool setupPhase;
        std::vector<Package*> top_packages;

        Package* makeTopPackage(Name* package);
    public:
        PackagesManager(std::map<std::string, std::vector<CompilationTable*> >& packagesCompilations);
        ~PackagesManager();

        Package* getPackageViaName(Name* package);
};

#endif
