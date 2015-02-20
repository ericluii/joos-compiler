#ifndef __BUILDAST_H__
#define __BUILDAST_H__

#include "ruleNumbers.h"
#include "parseTree.h"
#include <iostream>

#include "ast.h"
#include "compilationUnit.h"
#include "packageDecl.h"
#include "classDecl.h"
#include "identifier.h"
#include "interfaceDecl.h"
#include "name.h"
#include "typeDecl.h"
#include "modifiers.h"
#include "super.h"
#include "interfaceList.h"
#include "implements.h"
// Imports
#include "importDecls.h"
#include "importDeclsStar.h"
// Class body
#include "classBodyStar.h"
#include "classBodyDecls.h"
// Type
#include "type.h"
// Expression
#include "expressionStar.h"
#include "expression.h"
#include "assignment.h"
// Arguments
#include "arguments.h"
#include "argumentsStar.h"
// Method invocation
#include "methodInvoke.h"
// Access and Primary
#include "fieldAccess.h"
#include "arrayAccess.h"
// Class creation
#include "newClassCreation.h"
// Class method
#include "methodHeader.h"
#include "methodBody.h"
// Parameters
#include "formalParamStar.h"
#include "paramList.h"
// Block statements
#include "blockStmtsStar.h"
#include "blockStmts.h"
#include "stmtExpr.h"

class BuildAst
{
    private:
        static const bool debug = true;
        static CompilationUnit *makeCompilationUnit(ParseTree *tree);
        static PackageDecl *makePackageDecl(ParseTree *tree);
        static ImportDeclsStar *makeImportDeclsStar(ParseTree *tree);
        static ImportDecls *makeImportDecls(ParseTree *tree);
        static TypeDecl *makeTypeDecl(ParseTree *tree);
        static ClassDecl *makeClassDecl(ParseTree *tree);
        static InterfaceDecl *makeInterfaceDecl(ParseTree *tree);
        static Name *makeName(ParseTree *tree);
        static Identifier *makeIdentifier(ParseTree *tree);
        static Modifiers* makeModifiers(ParseTree* tree);
        static Super* makeSuper(ParseTree* tree);
        static InterfaceList* makeInterfaceList(ParseTree* tree);
        static Implements* makeImplements(ParseTree* tree);
        static ClassBodyStar* makeClassBodyStar(ParseTree* tree);
        static ClassBodyDecls* makeClassBodyDecls(ParseTree* tree);
        static ClassBodyDecls* makeClassBodyDecl(ParseTree* tree);
        static ClassBodyDecls* makeClassMember(ParseTree* tree);
        static MethodHeader* makeMethodHeader(ParseTree* tree);
        static MethodBody* makeMethodBody(ParseTree* tree);
        static FormalParamStar* makeFormalParamStar(ParseTree* tree);
        static ParamList* makeParamList(ParseTree* tree);
        static BlockStmtsStar* makeBlockStmtsStar(ParseTree* tree);
        static BlockStmts* makeBlockStmts(ParseTree* tree);
        static BlockStmts* makeSingleStmt(ParseTree* tree);
        static BlockStmts* makeStatement(ParseTree* tree);
        static BlockStmts* makeIfStmt(ParseTree* tree);
        static BlockStmts* makeWhileStmt(ParseTree* tree);
        static BlockStmts* makeForStmt(ParseTree* tree);
        static BlockStmts* makeNoTrailingSubstatement(ParseTree* tree);
        static StmtExpr* makeStmtExpr(ParseTree* tree);
        static Type* makeType(ParseTree* tree);
        static Type* makeReferenceType(ParseTree* tree);
        static ExpressionStar* makeExpressionStar(ParseTree* tree);
        static Expression* makeExpression(ParseTree* tree);
        static Expression* makeBinaryExpression(ParseTree* tree);
        static Expression* makeUnaryExpression(ParseTree* tree);
        static Expression* makeUnaryNotMinusExpr(ParseTree* tree);
        static Expression* makeCastExpression(ParseTree* tree);
        static Assignment* makeAssignment(ParseTree* tree);
        static Primary* makePrimary(ParseTree* tree);
        static Primary* makePrimaryNonArray(ParseTree* tree);
        static Primary* makePrimaryNewArray(ParseTree* tree);
        static NewClassCreation* makeClassCreation(ParseTree* tree);
        static ArgumentsStar* makeArgumentsStar(ParseTree* tree);
        static Arguments* makeArguments(ParseTree* tree);
        static MethodInvoke* makeMethodInvoke(ParseTree* tree);
        static FieldAccess* makeFieldAccess(ParseTree* tree);
        static ArrayAccess* makeArrayAccess(ParseTree* tree);
        static ClassBodyDecls* makeConstructor(ParseTree* tree);
    public:
        static Ast* build(ParseTree* tree);
        BuildAst() {}
};


#endif
