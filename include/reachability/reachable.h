#ifndef __REACHABLE_H__
#define __REACHABLE_H__

#include <map>
#include <string>
#include <vector>

#include "constExprVal.h"
#include "ruleNumbers.h"

class CompilationTable;
class Type;
class ClassDecl;
class ClassBodyStar;
class ClassBodyDecls;
class ClassMethod;
class MethodBody;
class BlockStmtsStar;
class BlockStmts;
class NestedBlock;
class IfStmt;
class WhileStmt;
class Expression;
class BinaryExpression;
class NegationExpression;
class PrimaryExpression;
class ForStmt;
class ReturnStmt;

class Constructor;

class Constructor;

class Reachable {
    private:
        std::map<std::string, std::vector<CompilationTable*> >& compilations;
        CompilationTable* curCompilation;
        
        // return type of currently seen method
        Type* curReturnType;
        // indicate that we're currently in a constructor
        bool inConstructor;
        // indicate that there is a statement inside a method/constructor
        // that cannot be reached
        bool unreachableStmt;
        CONST_EXPR_VAL recentConstCondExprVal;
        // the signature of the currently seen constructor or method
        std::string curSignature;

        void checkReachability(CompilationTable* table);
        void checkReachability(ClassDecl* aClass);
        void checkReachability(ClassBodyStar* body);
        void checkReachability(ClassBodyDecls* body);
        void checkReachability(ClassMethod* method);
        void checkReachability(MethodBody* body);
        void checkReachability(BlockStmtsStar* stmts);
        void checkReachability(BlockStmts* stmt);
        void checkReachability(NestedBlock* block);
        void checkReachability(IfStmt* stmt);
        void checkReachability(WhileStmt* stmt);
        void checkReachability(ForStmt* stmt);
        void checkReachability(ReturnStmt* stmt);

        void checkReachability(Constructor* ctor);

        // -------------------------------------------------------------------------
        // error reporting
        bool checkCompletionOfPrevStmt(BlockStmts* stmt);
        void reportUnreachableStatement(const std::string& stmtKind, const std::string& cause);
        void reportMissingReturnStatement();

        // -------------------------------------------------------------------------
        // checks whether an expression is a constant expression and evaluate its value
        CONST_EXPR_VAL checkConstExprAndVal(Expression* expr);
        CONST_EXPR_VAL checkConstExprAndVal(BinaryExpression* expr);
        CONST_EXPR_VAL checkConstExprAndVal(NegationExpression* expr);
        CONST_EXPR_VAL checkConstExprAndVal(PrimaryExpression* expr);

        // -------------------------------------------------------------------------
        // helper functions
        void convertConstantValueToIntOrChar(const std::string& constExprVal, int& n, char& c);
        int evaluateBinaryNumericOperation(const std::string& leftOp, const std::string& rightOp, RuleNumbers ruleOp);
    public:
        Reachable(std::map<std::string, std::vector<CompilationTable*> >& compilations);
        
        // --------------------------------------------------------------
        // Called by main
        void checkReachability();
};

#endif
