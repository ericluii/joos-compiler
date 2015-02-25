#include "buildCompilationTable.h"
#include <cassert>

BuildCompilationTable::BuildCompilationTable() : curSymTable(NULL), curCompTable(NULL) {}

BuildCompilationTable::~BuildCompilationTable() {}

void BuildCompilationTable::attachSymbolTable(SymbolTable* table) {
    if(curSymTable->isClassMethodTable()) {
        // if this was the class method table, then set the symbol table for the method
        ((ClassMethodTable*) curSymTable)->setSymTableOfMethod(table);
    } else if(curSymTable->isNestedBlockTable()) {
        ((NestedBlockTable*) curSymTable)->setSymTableOfBlock(table);
    } else if(curSymTable->isConstructorTable()) {
        ((ConstructorTable*) curSymTable)->setSymTableOfConstructor(table);
    } else if(curSymTable->isForTable()) {
        // If it ever reaches here then it means that the forInit of the previously seen
        // for statement wasn't a local variable declaration and the current table
        // is a NestedBlockTable
        assert(table->isNestedBlockTable());
        ((ForTable*) curSymTable)->setLoopTable((NestedBlockTable*) table);
    } else {
        curSymTable->setNextTable(table);
    }
    table->setPrevTable(curSymTable);
    curSymTable = table;
}

// -------------------------------------------------------
// Base nodes

void BuildCompilationTable::build(TypeDecl& node) {
    if(node.isClass()) {
        build((ClassDecl&) node);
    } else {
        // Interface declaration
        build((InterfaceDecl&) node);
    }
}

void BuildCompilationTable::build(ClassBodyStar& node) {
    if(!node.isEpsilon()) {
        build(*node.getBody());
    }
}

void BuildCompilationTable::build(ClassBodyDecls& node) {
    // building table by first going down the tree since the AST structure represents
    // the program in a reversed fashion
    if(!node.isLastClassMember()) {
        build(*node.getNextDeclaration());
    }

    if(node.isFieldDecl()) {
        build((FieldDecl&) node);
    } else if(node.isClassMethod()) {
        build((ClassMethod&) node);
    } else {
        // Constructor
        build((Constructor&) node);
    }
}

void BuildCompilationTable::build(MethodBody& node) {
    if(!node.noDefinition()) {
        build(*node.getBlockStmtsStar());
    }
}

void BuildCompilationTable::build(BlockStmtsStar& node) {
    if(!node.isEpsilon()) {
        build(*node.getStatements());
    }
}

void BuildCompilationTable::build(BlockStmts& node) {
    if(!node.isLastStatement()) {
        build(*node.getNextBlockStmt());
    }

    if(node.isLocalVarDecl()) {
        build((LocalDecl&) node);
    } else if(node.isIfStmt() || node.isIfThenElseStmt()) {
        build((IfStmt&) node);
    } else if(node.isWhileStmt()) {
        build((WhileStmt&) node);
    } else if(node.isForStmt()) {
        build((ForStmt&) node);
    } else if(node.isNestedBlock()) {
        build((NestedBlock&) node);
    }
}

// -------------------------------------------------------
// Class

void BuildCompilationTable::build(ClassDecl& node) {
    curSymTable = new ClassTable(&node);
    curCompTable->setSymbolTable(curSymTable);
    build(*node.getClassMembers());
}

void BuildCompilationTable::build(FieldDecl& node) {
    FieldTable* table = new FieldTable(&node);
    curSymTable->setNextTable(table);
    table->setPrevTable(curSymTable);
    curSymTable = table;
}

void BuildCompilationTable::build(ClassMethod& node) {
    ClassMethodTable* table = new ClassMethodTable(&node);
    curSymTable->setNextTable(table);
    table->setPrevTable(curSymTable);
    curSymTable = table;
    // temporary store
    SymbolTable* tempTable = curSymTable;
    build(*node.getMethodBody());
    curSymTable = tempTable;
}

void BuildCompilationTable::build(LocalDecl& node) {
    LocalTable* table = new LocalTable(&node);
    attachSymbolTable(table);    
}

void BuildCompilationTable::build(NestedBlock& node) {
    NestedBlockTable* table = new NestedBlockTable(&node);
    attachSymbolTable(table);
    // temporary store
    SymbolTable* tempTable = curSymTable;
    build(*node.getNestedBlock());
    curSymTable = tempTable;
}

void BuildCompilationTable::build(IfStmt& node) {
    build(*node.getExecuteTruePart());
    if(node.isIfThenElseStmt()) {
        build(*node.getExecuteFalsePart());
    }
}

void BuildCompilationTable::build(WhileStmt& node) {
    build(*node.getLoopStmt());
}

void BuildCompilationTable::build(ForStmt& node) {
    ForTable* table = new ForTable(&node);
    attachSymbolTable(table);
    LocalTable* localTable = NULL;
    SymbolTable* tempTable = curSymTable;
    if((node.getForInit())->isLocalVarDecl()) {
        // If the forInit is a local variable declaration
        localTable = new LocalTable((LocalDecl*) node.getForInit());
        curSymTable->setNextTable(localTable);
        localTable->setPrevTable(curSymTable);
        curSymTable = tempTable;
    }

    build(*node.getLoopStmt());
    curSymTable = tempTable;
    if(((ForTable*)curSymTable)->getForInitTable() != NULL) {
        // if the forInit was actually a local variable declaration
        // set the loop table in the for table
        ((ForTable*)curSymTable)->setLoopTable((NestedBlockTable*) localTable->getNextTable());
    }
}

void BuildCompilationTable::build(Constructor& node) {
    ConstructorTable* table = new ConstructorTable(&node);
    curSymTable->setNextTable(table);
    table->setPrevTable(curSymTable);
    curSymTable = table;
    // temporary store
    SymbolTable* tempTable = curSymTable;
    build(*node.getConstructorBody());
    curSymTable = tempTable;
}

// -------------------------------------------------------
// Interface

void BuildCompilationTable::build(InterfaceDecl& node) {
    curSymTable = new InterfaceTable(&node);
    curCompTable->setSymbolTable(curSymTable);
    build(*node.getInterfaceBodyStar());
}

void BuildCompilationTable::build(InterfaceBodyStar& node) {
    if(!node.isEpsilon()) {
        build(*node.getInterfaceMethods());
    }
}

void BuildCompilationTable::build(InterfaceMethod& node) {
    if(!node.isLastMethod()) {
        build(*node.getNextInterfaceMethod());
    }

    InterfaceMethodTable* table = new InterfaceMethodTable(&node);
    curSymTable->setNextTable(table);
    table->setPrevTable(curSymTable);
    curSymTable = table;
}

//--------------------------------------------------------

CompilationTable* BuildCompilationTable::build(CompilationUnit& node) {
    curCompTable = new CompilationTable(node.getPackageDecl());
    build(*node.getTypeDecl());
    return curCompTable;
}
