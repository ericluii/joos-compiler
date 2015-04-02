#ifndef __CODEGENERATOR_H__
#define __CODEGENERATOR_H__

#include <map>
#include <string>
#include <fstream>

class CompilationTable;
class Startup;

// Managers
class VTableManager;
class ObjectLayoutManager;
class InheritanceTableManager;
class ImplInterfaceMethodTableManager;
class StaticFieldsManager;
class SymbolTable;

// AST related
class ClassDecl;
class ClassBodyStar;
class ClassBodyDecls;
class FieldDecl;
class ClassMethod;
class Constructor;
class Expression;
class BinaryExpression;
class PrimaryExpression;
class Primary;
class ArrayAccess;
class Name;
class FieldAccess;
class MethodInvoke;
class ArgumentsStar;
class Arguments;
class NewClassCreation;
class PrimaryNewArray;
class LiteralOrThis;
class LiteralOrThis;
class NegationExpression;
class CastExpression;
class InstanceOf;
class Assignment;
class MethodBody;
class BlockStmtsStar;
class BlockStmts;
class LocalDecl;
class IfStmt;
class WhileStmt;
class ForStmt;
class StmtExpr;
class NestedBlock;
class ReturnStmt;
class ExpressionStar;

#define asmc(comment) *fs << ";; " << comment << "\n"
#define asmgl(global) *fs << "global " << global << "\n" << global << ":\n"
#define asml(label) *fs << "  " << label << ":\n"
#define asma(code) *fs << "\t" << code << "\n"

class CodeGenerator {
    private:
        std::map<std::string, CompilationTable*>& compilations;
        Startup* starter;
        VTableManager* virtualManager;
        ObjectLayoutManager* objManager;
        InheritanceTableManager* inhManager;
        ImplInterfaceMethodTableManager* interManager;
        StaticFieldsManager* staticManager;
        std::ofstream* fs;

        CompilationTable* processing;
        unsigned int scope_offset;
        std::map<void*, unsigned int> addressTable;

        // code generation through AST traversal
        void traverseAndGenerate(ClassDecl*);
        void traverseAndGenerate(ClassBodyStar*);
        void traverseAndGenerate(ClassBodyDecls*);
        void traverseAndGenerate(FieldDecl*);
        void traverseAndGenerate(Expression*);
        void traverseAndGenerate(BinaryExpression*);
        void traverseAndGenerate(PrimaryExpression*);
        void traverseAndGenerate(Primary*);
        void traverseAndGenerate(ArrayAccess*);
        void traverseAndGenerate(Name*);
        void traverseAndGenerate(FieldAccess*);
        void traverseAndGenerate(MethodInvoke*);
        void traverseAndGenerate(ArgumentsStar*);
        void traverseAndGenerate(Arguments*);
        void traverseAndGenerate(NewClassCreation*);
        void traverseAndGenerate(PrimaryNewArray*);
        void traverseAndGenerate(QualifiedThis*);
        void traverseAndGenerate(LiteralOrThis*);
        void traverseAndGenerate(NegationExpression*);
        void traverseAndGenerate(CastExpression*);
        void traverseAndGenerate(InstanceOf*);
        void traverseAndGenerate(Assignment*);
        void traverseAndGenerate(ClassMethod*);
        void traverseAndGenerate(MethodBody*);
        void traverseAndGenerate(Constructor*);
        void traverseAndGenerate(BlockStmtsStar*);
        void traverseAndGenerate(BlockStmts*);
        void traverseAndGenerate(LocalDecl*);
        void traverseAndGenerate(IfStmt*);
        void traverseAndGenerate(WhileStmt*);
        void traverseAndGenerate(ForStmt*);
        void traverseAndGenerate(ExpressionStar*);
        void traverseAndGenerate(StmtExpr*);
        void traverseAndGenerate(NestedBlock*);
        void traverseAndGenerate(ReturnStmt*);

        void* getSymbolTableForName(Name* name);
        // lol bad cade
        void CALL_FUNCTION(std::string fn_name);
    public:
        CodeGenerator(std::map<std::string, CompilationTable*>&, CompilationTable*);
        ~CodeGenerator();

        void generateStartFile();
        void traverseAndGenerate();
};

#endif
