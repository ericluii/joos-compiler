#include "singleTypeImportDecls.h"

SingleTypeImportDecl::SingleTypeImportDecl(Name *name, ImportDecls *nextImport){
    this->name = name;
    this->nextImport = nextImport;
}

Name *SingleTypeImportDecl::GetName(){
    return name;
}


ImportDecls *SingleTypeImportDecl::GetNextImport(){
    return nextImport;
}