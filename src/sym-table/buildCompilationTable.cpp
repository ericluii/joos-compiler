#include <sstream>
#include <cassert>
#include "buildCompilationTable.h"
#include "token.h"
#include "error.h"

BuildCompilationTable::BuildCompilationTable() : curSymTable(NULL), curCompTable(NULL) {}

BuildCompilationTable::~BuildCompilationTable() {}

void BuildCompilationTable::attachSymbolTable(SymbolTable* table) {
    if(curSymTable->isClassMethodTable()) {
        // if this was the class method table, then set the symbol table for the method
        ((ClassMethodTable*) curSymTable)->setSymTableOfMethod(table);
    } else if(curSymTable->isNestedBlockTable()) {
        if(!((NestedBlockTable*) curSymTable)->isTableSet()) {
            // if the nested block table has not been set
            ((NestedBlockTable*) curSymTable)->setSymTableOfBlock(table);
        } else {
            curSymTable->setNextTable(table);
        }
    } else if(curSymTable->isConstructorTable()) {
        ((ConstructorTable*) curSymTable)->setSymTableOfConstructor(table);
    } else if(curSymTable->isForTable()) {
        // If it ever reaches here, then it must mean that the loop statement
        // of the previously seen for statement is something with a nested block
        // OR another for statement itself
        if(!((ForTable*) curSymTable)->isTableSet()) {
            // if the for table has not been set
            assert(table->isNestedBlockTable() || table->isForTable());
            ((ForTable*) curSymTable)->setLoopTable(table);
        } else {
            curSymTable->setNextTable(table);
        }
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
    } else if(node.isInterface()) {
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

    if(node.isField()) {
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
    node.setClassTable((ClassTable*)curSymTable);
    build(*node.getClassMembers());
}

void BuildCompilationTable::build(FieldDecl& node) {
    FieldTable* table = new FieldTable(&node, curCompTable);
    curSymTable->setNextTable(table);
    table->setPrevTable(curSymTable);
    
    node.setFieldTable(table);
    curCompTable->registerAField(node.getFieldDeclared()->getIdAsString(), table); 
    curSymTable = table;
}

void BuildCompilationTable::build(ClassMethod& node) {
    ClassMethodTable* table = new ClassMethodTable(&node, curCompTable);
    curSymTable->setNextTable(table);
    table->setPrevTable(curSymTable);
    curSymTable = table;
    // temporary store
    SymbolTable* tempTable = curSymTable;
    build(*node.getMethodHeader());
    build(*node.getMethodBody());
    
    node.setClassMethodTable(table);
    curSymTable = tempTable;
}

void BuildCompilationTable::build(MethodHeader& node) {
    build(*node.getClassMethodParams());
}

void BuildCompilationTable::build(FormalParamStar& node) {
    if(!node.isEpsilon()) {
        build(*node.getListOfParameters());
    }
}

void BuildCompilationTable::build(ParamList& node) {
    if(!node.isLastParameter()) {
        build(*node.getNextParameter());
    }

    ParamTable* table = new ParamTable(&node);
    node.setParamTable(table);
    attachSymbolTable(table);
}

void BuildCompilationTable::build(LocalDecl& node) {
    LocalTable* table = new LocalTable(&node);
    node.setLocalTable(table);
    attachSymbolTable(table);
}

void BuildCompilationTable::build(NestedBlock& node) {
    NestedBlockTable* table = new NestedBlockTable(&node);
    attachSymbolTable(table);
    // temporary store
    SymbolTable* tempTable = curSymTable;
    build(*node.getNestedBlock());
    table->indicateTableIsSet();
    
    node.setNestedBlockTable(table);
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
    
    if(!node.emptyForInit()) {
        if((node.getForInit())->isLocalVarDecl()) {
            // If the forInit is a local variable declaration
            localTable = new LocalTable((LocalDecl*) node.getForInit());
            ((ForTable*)curSymTable)->setForInitTable(localTable);
            localTable->setPrevTable(curSymTable);
            ((LocalDecl*) node.getForInit())->setLocalTable(localTable);
        }
    }

    build(*node.getLoopStmt());
    table->indicateTableIsSet();
    node.setForTable(table);
    curSymTable = table;
    if(table->getForInitTable() != NULL) {
        // If forInit was a local variable declaration, then link them together
        table->getForInitTable()->setNextTable(table->getLoopTable());
        if(table->getLoopTable() != NULL) {
            table->getLoopTable()->setPrevTable(table->getForInitTable());
        }
    } else if(table->getLoopTable() != NULL) {
        // If the forInit wasn't a local variable declaration, then link the loop table
        // with the for table if the loop table isn't NULL
        table->getLoopTable()->setPrevTable(table);
    }
}

void BuildCompilationTable::build(Constructor& node) {
    ConstructorTable* table = new ConstructorTable(&node, curCompTable);
    curSymTable->setNextTable(table);
    table->setPrevTable(curSymTable);
    curSymTable = table;
    // temporary store
    SymbolTable* tempTable = curSymTable;
    build(*node.getConstructorParameters());
    build(*node.getConstructorBody());
    
    node.setConstructorTable(table);
    curSymTable = tempTable;
}

// -------------------------------------------------------
// Interface

void BuildCompilationTable::build(InterfaceDecl& node) {
    curSymTable = new InterfaceTable(&node);
    curCompTable->setSymbolTable(curSymTable);
    node.setInterfaceTable((InterfaceTable*) curSymTable);
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
   
    // note that the parameter list for interface methods
    // do not have a symbol table because there's really no point in having them
    InterfaceMethodTable* table = new InterfaceMethodTable(&node, curCompTable);
    curSymTable->setNextTable(table);
    table->setPrevTable(curSymTable);
    
    node.setInterfaceMethodTable(table);
    curSymTable = table;
}

//--------------------------------------------------------

CompilationTable* BuildCompilationTable::build(CompilationUnit& node, const std::string& filename) {
    curCompTable = new CompilationTable(node.getPackageDecl(), filename, &node);
    if(!(node.getTypeDecl()->isEpsilon())) {
        build(*node.getTypeDecl());
    }
    return curCompTable;
}
