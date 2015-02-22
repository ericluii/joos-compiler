#ifndef __ASTPRINTER_H__
#define __ASTPRINTER_H__

#include "compilationUnit.h"
#include "packageDecl.h"
#include "importDeclsStar.h"
#include "typeDecl.h"

class AstPrinter {
    private:
        static unsigned int depth = 0;
        static void print();
    public:
        static void print(CompilationUnit& node);
};

#endif
