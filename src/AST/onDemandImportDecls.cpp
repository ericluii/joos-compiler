#include "onDemandImportDecls.h"

OnDemandImportDecl::OnDemandImportDecl(Name *name, ImportDecls *nextImport){
    this->name = name;
    this->nextImport = nextImport;
}

Name *OnDemandImportDecl::GetName(){
    return name;
}


ImportDecls *OnDemandImportDecl::GetNextImport(){
    return nextImport;
}