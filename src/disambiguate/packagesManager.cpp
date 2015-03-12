#include "packagesManager.h"
#include "packageDecl.h"
#include "package.h"
#include "compilationTable.h"

PackagesManager::PackagesManager(std::map<std::string, std::vector<CompilationTable*> >& packagesCompilations) : setupPhase(true) {
    std::map<std::string, std::vector<CompilationTable*> >::iterator it;
    for(it = packagesCompilations.begin(); it != packagesCompilations.end(); it++) {
        // grab the first thing, there will at least be one
        PackageDecl* pkgDecl = it->second.at(0)->getPackageRawForm();
        pkgDecl->getPackageName();
        Package* existingPackage = getPackageViaName(pkgDecl->getPackageName());
        if(existingPackage == NULL) {
            // if a package with the same name or a prefix of it
            // has not been found
            makeTopPackage(pkgDecl->getPackageName());
            // now that it has been inserted, it can be found
            existingPackage = getPackageViaName(pkgDecl->getPackageName());
        }

        std::vector<CompilationTable*>::iterator it2;
        for(it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            existingPackage->registerCompilation(*it2);
        }
    }
    setupPhase = false;
}

PackagesManager::~PackagesManager() {
    std::vector<Package*>::iterator it;
    for(it = top_packages.begin(); it != top_packages.end(); it++) {
        delete *it;
    }
}

Package* PackagesManager::makeTopPackage(Name* package) {
    Package* retPkg = NULL;
    if(package == NULL) {
        // then this must be the default package
        retPkg = new Package("");
        top_packages.push_back(retPkg);
        return retPkg;
    }

    if(!package->isLastPrefix()) {
        // a qualified name
        retPkg = makeTopPackage(package->getNextName());
    } else {
        // a simplified name, i.e the end
        retPkg = new Package(package->getNameId()->getIdAsString());
        top_packages.push_back(retPkg);
        return retPkg;
    }
    Package* subPkg = new Package(package->getNameId()->getIdAsString());
    retPkg->insertSubpackage(subPkg);
    return subPkg;
}

Package* PackagesManager::getPackageViaName(Name* package) {
    // if package is NULL, then it must be the default package
    if(package == NULL) { 
        std::vector<Package*>::iterator it;
        for(it = top_packages.begin(); it != top_packages.end(); it++) {
            if((*it)->getPkgNameAsString() == "") {
                return *it;
            }
        }
        return NULL;
    }
    
    Package* somePackage = NULL;
    if(!package->isLastPrefix()) {
        // a qualified name
        somePackage = getPackageViaName(package->getNextName());
    } else {
        // a simplified name, i.e the end
        std::vector<Package*>::iterator it;
        std::string pkgName = package->getNameId()->getIdAsString();
        for(it = top_packages.begin(); it != top_packages.end(); it++) {
            if((*it)->getPkgNameAsString() == pkgName) {
                // the package name is the same
                return *it;
            }
        }
        // couldn't find the package
        return NULL;
    }

    if(somePackage == NULL) {
        // just return NULL if it's NULL
        return somePackage;
    }

    Package* tmpPackage = somePackage->getSubpackageWithName(package->getNameId()->getIdAsString());
    if(setupPhase) {
        // only do this during the setup phase
        if(tmpPackage == NULL) {
            // couldn't find it as a subpackage, then it must be
            // an unregistered subpackage of an existing package
            tmpPackage = new Package(package->getNameId()->getIdAsString());
            somePackage->insertSubpackage(tmpPackage);
        }
    }
    return tmpPackage;
}
