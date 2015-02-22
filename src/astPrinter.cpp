#include <iostream>
#include "astPrinter.h"

unsigned int AstPrinter::depth = 0;

void AstPrinter::print(CompilationUnit& node) {
    std::cout << node.getLexeme() << std::endl;
    depth+= 1;
    print(*(node.getPackageDecl()));
    print(*(node.getImportDeclsStar()));
    print(*(node.getTypeDecl()));
}

void AstPrinter::print(PackageDecl& node) {
    for(unsigned int i = 0; i < depth; i++) {
        std::cout << ' ' << std::endl;
}

void AstPrinter::print(ImportDeclsStar& node) {
}

void AstPrinter::print(TypeDecl& node) {
}

void AstPrinter::print(Name& node) {
}
