#ifndef __AMBIGUOUSLINKER_H__
#define __AMBIGUOUSLINKER_H__

#include <map>
#include <vector>
#include <string>

class PackagesManager;
class CompilationTable;
class SymbolTable;
class FieldTable;

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

class Token;

class AmbiguousLinker {
    private:
        PackagesManager& manager;
        std::map<std::string, std::vector<CompilationTable*> >& compilations;
        SymbolTable* curSymTable;
        CompilationTable* curCompilation;

        // -----------------------------------------------------------------
        // Class part
        void traverseAndLink(ClassDecl* aClass);
        void traverseAndLink(ClassBodyStar* body);
        void traverseAndLink(ClassBodyDecls* body);
        void traverseAndLink(FieldDecl* field);
        void traverseAndLink(Expression* expr, bool withinMethod);
        void traverseAndLink(Assignment* assign, bool withinMethod);
        void traverseAndLink(AssignName* assign, bool withinMethod);
        void traverseAndLink(Name* name, bool withinMethod);
        void traverseAndLink(AssignField* assign, bool withinMethod);
        void traverseAndLink(FieldAccess* assign, bool withinMethod);
        void traverseAndLink(Primary* prim, bool withinMethod);
        void traverseAndLink(LiteralOrThis* lit);
        void traverseAndLink(NewClassCreation* create, bool withinMethod);
        void traverseAndLink(ArgumentsStar* args, bool withinMethod);
        void traverseAndLink(Arguments* args, bool withinMethod);
        void traverseAndLink(PrimaryNewArray* newArray, bool withinMethod);
        void traverseAndLink(MethodInvoke* invoke, bool withinMethod);
        void traverseAndLink(MethodNormalInvoke* invoke, bool withinMethod);
        void traverseAndLink(InvokeAccessedMethod* invoke, bool withinMethod);
        void traverseAndLink(ArrayAccess* access, bool withinMethod);
        void traverseAndLink(ArrayAccessName* access, bool withinMethod);
        void traverseAndLink(ArrayAccessPrimary* access, bool withinMethod);
        void traverseAndLink(AssignArray* assign, bool withinMethod);
        void traverseAndLink(CastExpression* cast, bool withinMethod);
        void traverseAndLink(CastName* cast, bool withinMethod);
        void traverseAndLink(CastPrimitive* cast);

        // ------------------------------------------------------------------
        // Interface part
        void traverseAndLink(InterfaceDecl* anInterface);
        
        // ------------------------------------------------------------------
        // starts from here
        void traverseAndLink(CompilationTable* compilation);

        // ------------------------------------------------------------------
        // error reporting
        void reportIllegalPrimitiveMemberAccess(const std::string& memberName, PrimitiveType* type, Token* tok);
        void reportIllegalArrayMemberAccess(const std::string& memberName, Type* type, Token* tok);

        // ------------------------------------------------------------------
        // Various helpers
        bool checkProperMemberAccessingFromVariable(const std::string& currName, Type* type, Token* tok);
        bool checkTypeIsClassDuringStaticAccess(CompilationTable* typeTable, const std::string& fullName, Token* tok);
        FieldTable* findFieldPreviouslyDeclared(const std::string& fieldName);
        CompilationTable* findTypeFromSingleImportsAndPackage(const std::string& typeName, Token* tok);
        void linkQualifiedName(Name* name);
        void linkSimpleName(Name* name, bool withinMethod);
        FieldTable* getStaticFieldInAClass(CompilationTable* someClass, const std::string& findField, Token* tok);
        CompilationTable* retrieveCompilation(const std::string& package, const std::string& typeName);
        bool setNameReferringToArrayLength(Name* name, Type* type);
        bool setFieldAccessReferringToArrayLength(FieldAccess* access, Type* type);
        int returnEvalTypeFromTypeNode(Type* type);
        void setExpressionTypeBasedOnType(Expression* expr, Type* type);
        void checkProperArrayAccessInExpression(Expression* expr, Token* tok);
        void setExpressionTypeBasedOnName(Expression* expr, Name* name);
        Token* setExpressionTypeBasedOnPrimary(Expression* expr, Primary* prim);
    public:
        AmbiguousLinker(PackagesManager& manager, std::map<std::string, std::vector<CompilationTable*> >& compilations);
        ~AmbiguousLinker();

        void performLinking();
};

#endif
