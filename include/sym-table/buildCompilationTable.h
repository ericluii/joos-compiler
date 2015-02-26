#ifndef __BUILDCOMPILATIONTABLE_H__
#define __BUILDCOMPILATIONTABLE_H__

// Symbol table
#include "classTable.h"
#include "constructorTable.h"
#include "forTable.h"
#include "interfaceTable.h"
#include "nestedBlockTable.h"
#include "classMethodTable.h"
#include "compilationTable.h"
#include "fieldTable.h"
#include "interfaceMethodTable.h"
#include "localTable.h"
#include "compilationTable.h"
// Base classes
#include "typeDecl.h"
#include "classBodyDecls.h"
#include "blockStmts.h"
// Class
#include "compilationUnit.h"
#include "classDecl.h"
#include "classBodyStar.h"
#include "fieldDecl.h"
#include "classMethod.h"
#include "methodBody.h"
#include "blockStmtsStar.h"
#include "forStmt.h"
#include "ifStmt.h"
#include "localDecl.h"
#include "nestedBlock.h"
#include "whileStmt.h"
#include "constructor.h"
// Interfaces
#include "interfaceDecl.h"
#include "interfaceBodyStar.h"
#include "interfaceMethod.h"

class BuildCompilationTable {
    private:
        SymbolTable* curSymTable;
        CompilationTable* curCompTable;

        void attachSymbolTable(SymbolTable* attach);
// Base classes
        void build(TypeDecl& node);
        void build(ClassBodyDecls& node);
        void build(BlockStmts& node);
// --------------------------------------------------
// --------------------------------------------------
// Class
        void build(ClassDecl& node);
        void build(ClassBodyStar& node);
        void build(FieldDecl& node);
        void build(ClassMethod& node);
        void build(MethodBody& node);
        void build(BlockStmtsStar& node);
        void build(ForStmt& node);
        void build(IfStmt& node);
        void build(LocalDecl& node);
        void build(NestedBlock& node);
        void build(WhileStmt& node);
        void build(Constructor& node);
// ----------------------------------------------------
// ----------------------------------------------------
// Interface
        void build(InterfaceDecl& node);
        void build(InterfaceBodyStar& node);
        void build(InterfaceMethod& node);
    public:
        CompilationTable* build(CompilationUnit& node, const std::string& filename);
        BuildCompilationTable();
        ~BuildCompilationTable();
};

#endif
