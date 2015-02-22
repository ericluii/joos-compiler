#ifndef __ASTPRINTER_H__
#define __ASTPRINTER_H__

#include "compilationUnit.h"
#include "packageDecl.h"
#include "importDeclsStar.h"
#include "typeDecl.h"
#include "name.h"

class AstPrinter {
    private:
        static unsigned int depth;
        static void print(PackageDecl& node);
        static void print(ImportDeclsStar& node);
        static void print(TypeDecl& node);
        static void print(Name& node);
    public:
        static void print(CompilationUnit& node);
};

#endif
