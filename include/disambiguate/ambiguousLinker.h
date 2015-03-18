#ifndef __AMBIGUOUSLINKER_H__
#define __AMBIGUOUSLINKER_H__

#include <map>
#include <vector>
#include <string>
#include "evaluatedType.h"

class PackagesManager;
class CompilationTable;
class SymbolTable;
class FieldTable;
class LocalTable;

class ClassDecl;
class InterfaceDecl;
class ClassBodyStar;
class ClassBodyDecls;
class FieldDecl;
class Expression;
class Assignment;
class InterfaceDecl;
class AssignName;
class AssignField;
class FieldAccess;
class Primary;
class LiteralOrThis;
class BracketedExpression;
class NewClassCreation;
class ArgumentsStar;
class Arguments;
class AssignArray;
class Name;
class Type;
class PrimitiveType;
class PrimaryNewArray;
class MethodInvoke;
class MethodNormalInvoke;
class InvokeAccessedMethod;
class ArrayAccess;
class ArrayAccessName;
class ArrayAccessPrimary;
class CastExpression;
class CastName;
class CastPrimitive;
class ClassMethod;
class MethodBody;
class BlockStmtsStar;
class BlockStmts;
class LocalDecl;
class ExpressionStar;
class StmtExpr;
class IfStmt;
class WhileStmt;
class ForStmt;
class NestedBlock;
class Constructor;
class ReferenceType;

class Token;

class AmbiguousLinker {
    private:
        PackagesManager& manager;
        std::map<std::string, std::vector<CompilationTable*> >& compilations;
        SymbolTable* curSymTable;
        CompilationTable* curCompilation;
        // indicates if linking is done in an initializer of a static field in this case
        bool asgmtFieldContext;
        // indicates if linking happens during the initializer of a local variable
        bool asgmtLocalContext;
        // indicates if linking happens in a class method
        bool withinMethod;
        // indicates the local variable being assigned
        std::string assigningLocalVar;

        // -----------------------------------------------------------------
        // Class part
        void traverseAndLink(ClassDecl* aClass);
        void traverseAndLink(ClassBodyStar* body);
        void traverseAndLink(ClassBodyDecls* body);
        void traverseAndLink(FieldDecl* field);
        void traverseAndLink(Expression* expr);
        void traverseAndLink(Assignment* assign);
        void traverseAndLink(AssignName* assign);
        void traverseAndLink(Name* name);
        void traverseAndLink(AssignField* assign);
        void traverseAndLink(FieldAccess* assign);
        void traverseAndLink(Primary* prim);
        void traverseAndLink(BracketedExpression* brkExpr);
        void traverseAndLink(LiteralOrThis* lit);
        void traverseAndLink(NewClassCreation* create);
        void traverseAndLink(ArgumentsStar* args);
        void traverseAndLink(Arguments* args);
        void traverseAndLink(PrimaryNewArray* newArray);
        void traverseAndLink(MethodInvoke* invoke);
        void traverseAndLink(MethodNormalInvoke* invoke);
        void traverseAndLink(InvokeAccessedMethod* invoke);
        void traverseAndLink(ArrayAccess* access);
        void traverseAndLink(ArrayAccessName* access);
        void traverseAndLink(ArrayAccessPrimary* access);
        void traverseAndLink(AssignArray* assign);
        void traverseAndLink(CastExpression* cast);
        void traverseAndLink(CastName* cast);
        void traverseAndLink(CastPrimitive* cast);
        void traverseAndLink(ClassMethod* method);
        void traverseAndLink(MethodBody* body);
        void traverseAndLink(BlockStmtsStar* block);
        void traverseAndLink(BlockStmts* stmts);
        void traverseAndLink(LocalDecl* local);
        void traverseAndLink(ExpressionStar* exprStar);
        void traverseAndLink(StmtExpr* stmt);
        void traverseAndLink(IfStmt* stmt);
        void traverseAndLink(WhileStmt* stmt);
        void traverseAndLink(ForStmt* stmt);
        void traverseAndLink(NestedBlock* nested);
        void traverseAndLink(Constructor* ctor);
        
        // ------------------------------------------------------------------
        // starts from here
        void traverseAndLink(CompilationTable* compilation);

        // ------------------------------------------------------------------
        // error reporting
        void reportIllegalPrimitiveMemberAccess(const std::string& memberName, PrimitiveType* type, Token* tok);
        void reportIllegalArrayMemberAccess(const std::string& memberName, Type* type, Token* tok);
        void reportIncompletePackageNameUsage(const std::string& pkgName, Token* tok);
        void reportLiteralDereferencing(LiteralOrThis* lit, Token* tok);

        // ------------------------------------------------------------------
        // Various helpers
        bool checkProperMemberAccessingFromVariable(const std::string& currName, Type* type, Token* tok);
        bool checkTypeIsClassDuringStaticAccess(CompilationTable* typeTable, const std::string& fullName, Token* tok);
        FieldTable* findFieldDeclaredInClass(const std::string& fieldName, bool previous);
        CompilationTable* findTypeFromSingleImportsAndPackage(const std::string& typeName, Token* tok);
        void linkQualifiedName(Name* name);
        void linkSimpleName(Name* name);
        FieldTable* getFieldInAClass(CompilationTable* someClass, const std::string& findField, Token* tok);
        CompilationTable* retrieveCompilation(const std::string& package, const std::string& typeName);
        bool setNameReferringToArrayLength(Name* name, Type* type);
        bool setFieldAccessReferringToArrayLength(FieldAccess* access, Type* type);
        int returnEvalTypeFromTypeNode(Type* type);
        void setExpressionTypeBasedOnType(Expression* expr, Type* type);
        void checkProperArrayAccessInExpression(Expression* expr, Token* tok);
        void setExpressionTypeBasedOnName(Expression* expr, Name* name);
        Token* setExpressionTypeBasedOnPrimary(Expression* expr, Primary* prim);
        SymbolTable* findLocalVarOrParameterPreviouslyDeclared(const std::string& currName);
        void setMethodForMethodInvokeFromCompilation(MethodInvoke* invoke, CompilationTable* someType,
                        const std::string& methodSignature, Token* tok);
        void setMethodForMethodInvokeFromType(Type* type, MethodInvoke* invoke, const std::string& methodSignature, Token* tok);
        bool checkProperMethodOrConstructorSignature(const std::string& methodSignature, Token* tok);
        bool checkProperArrayAccess(ArrayAccess* access, CompilationTable* table, EVALUATED_TYPE accessType, Token* tok);
        std::string getCorrespongindTypeString(EVALUATED_TYPE type, CompilationTable* table);
        Token* getTokenFromPrimary(Primary* prim);
        void linkNameToFieldFromType(Name* name, ReferenceType* type, const std::string& fieldName);
        void linkFieldAccessFromType(FieldAccess* access, ReferenceType* type, const std::string& fieldName);
        void linkFieldAccessFromCompilation(FieldAccess* access, CompilationTable* someClass,
                    const std::string& fieldName, Token* tok);
    public:
        AmbiguousLinker(PackagesManager& manager, std::map<std::string, std::vector<CompilationTable*> >& compilations);
        ~AmbiguousLinker();

        void performLinking();
};

#endif
