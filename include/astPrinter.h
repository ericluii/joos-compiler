#ifndef __ASTPRINTER_H__
#define __ASTPRINTER_H__

// Base classes
#include "typeDecl.h"
#include "classBodyDecls.h"
#include "expression.h"
#include "assignment.h"
#include "type.h"
#include "primary.h"
#include "methodInvoke.h"
#include "castExpression.h"
#include "assignment.h"
#include "arrayAccess.h"
#include "blockStmts.h"
#include "stmtExpr.h"
// Class
#include "compilationUnit.h"
#include "packageDecl.h"
#include "importDeclsStar.h"
#include "classDecl.h"
#include "interfaceDecl.h"
#include "name.h"
#include "identifier.h"
#include "importDecls.h"
#include "modifiers.h"
#include "super.h"
#include "interfaceList.h"
#include "interfaces.h"
#include "classBodyStar.h"
#include "fieldDecl.h"
#include "primitiveType.h"
#include "referenceType.h"
#include "binaryExpression.h"
#include "assignName.h"
#include "arrayAccessName.h"
#include "arrayAccessPrimary.h"
#include "literalOrThis.h"
#include "bracketedExpression.h"
#include "primaryExpression.h"
#include "assignArray.h"
#include "methodNormalInvoke.h"
#include "invokeAccessedMethod.h"
#include "argumentsStar.h"
#include "arguments.h"
#include "newClassCreation.h"
#include "primaryNewArray.h"
#include "castName.h"
#include "castPrimitive.h"
#include "instanceof.h"
#include "nameExpression.h"
#include "negationExpression.h"
#include "assignField.h"
#include "classMethod.h"
#include "methodHeader.h"
#include "formalParamStar.h"
#include "paramList.h"
#include "methodBody.h"
#include "blockStmtsStar.h"
#include "emptyStmt.h"
#include "forStmt.h"
#include "expressionStar.h"
#include "stmtExprAssign.h"
#include "stmtExprCreation.h"
#include "stmtExprInvoke.h"
#include "ifStmt.h"
#include "localDecl.h"
#include "nestedBlock.h"
#include "returnStmt.h"
#include "whileStmt.h"
#include "constructor.h"
#include "qualifiedThis.h"
// Interfaces
#include "interfaceDecl.h"
#include "interfaceBodyStar.h"
#include "interfaceMethod.h"
#include "modifiersStar.h"

class AstPrinter {
    private:
        static unsigned int depth;
        static void printSpaces();
// Base classes
        static void print(TypeDecl& node);
        static void print(ClassBodyDecls& node);
        static void print(Expression& node);
        static void print(Type& node);
        static void print(Primary& node);
        static void print(MethodInvoke& node);
        static void print(CastExpression& node);
        static void print(Assignment& node);
        static void print(ArrayAccess& node);
        static void print(BlockStmts& node);
        static void print(StmtExpr& node);
// --------------------------------------------------
// --------------------------------------------------
// Class
        static void print(PackageDecl& node);
        static void print(ImportDeclsStar& node);
        static void print(ClassDecl& node);
        static void print(Name& node);
        static void print(Identifier& node);
        static void print(ImportDecls& node);
        static void print(Modifiers& node);
        static void print(Super& node);
        static void print(InterfaceList& node);
        static void print(Interfaces& node);
        static void print(ClassBodyStar& node);
        static void print(FieldDecl& node);
        static void print(PrimitiveType& node);
        static void print(ReferenceType& node);
        static void print(BinaryExpression& node);
        static void print(LiteralOrThis& node);
        static void print(BracketedExpression& node);
        static void print(PrimaryExpression& node);
        static void print(MethodNormalInvoke& node);
        static void print(InvokeAccessedMethod& node);
        static void print(ArgumentsStar& node);
        static void print(Arguments& node);
        static void print(NewClassCreation& node);
        static void print(PrimaryNewArray& node);
        static void print(CastPrimitive& node);
        static void print(CastName& node);
        static void print(InstanceOf& node);
        static void print(NameExpression& node);
        static void print(NegationExpression& node);
        static void print(AssignName& node);
        static void print(AssignArray& node);
        static void print(ArrayAccessName& node);
        static void print(ArrayAccessPrimary& node);
        static void print(AssignField& node);
        static void print(FieldAccess& node);
        static void print(ClassMethod& node);
        static void print(MethodHeader& node);
        static void print(FormalParamStar& node);
        static void print(ParamList& node);
        static void print(MethodBody& node);
        static void print(BlockStmtsStar& node);
        static void print(EmptyStmt& node);
        static void print(ForStmt& node);
        static void print(ExpressionStar& node);
        static void print(StmtExprAssign& node);
        static void print(StmtExprCreation& node);
        static void print(StmtExprInvoke& node);
        static void print(IfStmt& node);
        static void print(LocalDecl& node);
        static void print(NestedBlock& node);
        static void print(ReturnStmt& node);
        static void print(WhileStmt& node);
        static void print(Constructor& node);
        static void print(QualifiedThis& node);
// ----------------------------------------------------
// ----------------------------------------------------
// Interface
        static void print(InterfaceDecl& node);
        static void print(InterfaceBodyStar& node);
        static void print(InterfaceMethod& node);
        static void print(ModifiersStar& node);
    public:
        static void print(CompilationUnit& node);
};

#endif
