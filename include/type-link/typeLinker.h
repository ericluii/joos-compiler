#ifndef __TYPELINKER_H__
#define __TYPELINKER_H__

#include <map>
#include <vector>
#include <string>
#include "compilationTable.h"

class PackageDecl;
class ImportDecls;
class CompilationUnit;
class ClassDecl;
class InterfaceDecl;
class Super;
class InterfaceList;
class Interfaces;
class ClassBodyStar;
class ClassBodyDecls;
class FieldDecl;
class ClassMethod;
class Constructor;
class Type;
class Expression;
class InstanceOf;
class Assignment;
class FieldAccess;
class Primary;
class ArrayAccess;
class MethodInvoke;
class NewClassCreation;
class PrimaryExpression;
class CastExpression;
class MethodHeader;
class MethodBody;
class FormalParamStar;
class ParamList;
class BlockStmtsStar;
class BlockStmts;
class ExpressionStar;
class StmtExpr;

class TypeLinker {
    private:
        std::map<std::string, std::vector<CompilationTable*> >& packages;
        // Related to single type imports clashing with types in the same file
        void checkSingleImportClashError(const std::string& typeName, const std::string& packageName, ImportDecls* imports);
        void checkImportsClashWithType(const std::string& typeName, const std::string& packageName, ImportDecls* imports);
        void checkForClashingSingleImportInFile(CompilationTable*);

        // Related to checking for imported types or packages to exist
        void setImportIfExist(CompilationTable* compilation, ImportDecls* import);
        void importsExistenceRecurse(CompilationTable* compilation, ImportDecls* import);
        void checkImportsExistenceAndSet(CompilationTable* compilation);

        // Related to whether a name conflicts with a type
        bool checkIfNameConflictsWithType(CompilationTable* compilation, Name* name, bool isType);

        // Related to checking if packages or prefixes of packages/imports resolve to types
        void checkImportsResolveToTypes(CompilationTable* compilation, ImportDecls* import);
        void checkPackageResolveToTypes(CompilationTable* compilation, PackageDecl* package);
        void checkPackageAndImportsResolveToTypes(CompilationTable* compilation);

        // Related to making sure all names that resolve to types refer to some class or interface
        void linkTypeNames(CompilationTable* compilation, CompilationUnit* unit);
        void linkTypeNames(CompilationTable* compilation, ClassDecl* type);
        void linkTypeNames(CompilationTable* compilaiton, Super* super);
        void linkTypeNames(CompilationTable* compilation, InterfaceList* interfaces);
        void linkTypeNames(CompilationTable* compilation, Interfaces* interface);
        void linkTypeNames(CompilationTable* compilation, ClassBodyStar* classBody);
        void linkTypeNames(CompilationTable* compilation, ClassBodyDecls* classMember);
        void linkTypeNames(CompilationTable* compilation, FieldDecl* field);
        void linkTypeNames(CompilationTable* compilation, Type* type);
        void linkTypeNames(CompilationTable* compilation, Expression* expr);
        void linkTypeNames(CompilationTable* compilation, Assignment* assign);
        void linkTypeNames(CompilationTable* compilation, FieldAccess* fieldAccessed);
        void linkTypeNames(CompilationTable* compilation, ArrayAccess* array);
        void linkTypeNames(CompilationTable* compilation, PrimaryExpression* primExpr);
        void linkTypeNames(CompilationTable* compilation, Primary* prim);
        void linkTypeNames(CompilationTable* compilation, MethodInvoke* invoke);
        void linkTypeNames(CompilationTable* compilation, NewClassCreation* create);
        void linkTypeNames(CompilationTable* compilation, CastExpression* castExpr);
        void linkTypeNames(CompilationTable* compilation, ClassMethod* method);
        void linkTypeNames(CompilationTable* compilation, MethodHeader* header);
        void linkTypeNames(CompilationTable* compilation, FormalParamStar* params);
        void linkTypeNames(CompilationTable* compilation, ParamList* params);
        void linkTypeNames(CompilationTable* compilation, MethodBody* body);
        void linkTypeNames(CompilationTable* compilation, BlockStmtsStar* stmts);
        void linkTypeNames(CompilationTable* compilation, BlockStmts* stmts);
        void linkTypeNames(CompilationTable* compilation, ExpressionStar* exprStar);
        void linkTypeNames(CompilationTable* compilation, StmtExpr* stmtExpr);
        void linkTypeNames(CompilationTable* compilation, Constructor* ctor);

        void linkTypeNames(CompilationTable* compilation, InterfaceDecl* type);

        CompilationTable* linkTypeNames(CompilationTable* compilation, Name* name);
        
        // Related to reporting errors regarding to type names not being able to be linked
        void reportTypeNameLinkError(const std::string& errorMsg, const std::string& typeName, Token* tok);
    public:
        TypeLinker(std::map<std::string, std::vector<CompilationTable*> >& packages);
        ~TypeLinker();

        void typeLinkingResolution();
};

#endif
