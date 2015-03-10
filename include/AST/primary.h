#ifndef __PRIMARY_H__
#define __PRIMARY_H__

#include "ast.h"
#include "ambiguousLinkType.h"

class CompilationTable;
class FieldTable;
class ClassMethodTable;
class InterfaceMethodTable;
class ParamTable;
class LocalTable;
class ConstructorTable;

class Primary : public Ast {
    protected:
        // below are the tables that each Primary AST node may have
        // Only possible in LiteralOrThis, as this, or as a string literal
        // Can only be a class due to Primarys being only in a class and a string literal
        // refers to java.lang.String which is a class (stdlib assumption)
        CompilationTable* someClass; 
        FieldTable* fieldTable; // FieldAccess
        ClassMethodTable* classMethod; // MethodNormalInvoke, InvokeAccessedMethod
        InterfaceMethodTable* interfaceMethod; // MethodNormalInvoke, InvokeAccessedMethod
        ParamTable* paramTable; // ArrayAccessName
        LocalTable* localTable; // ArrayAccessName
        ConstructorTable* ctorTable; // NewClassCreation

        AMBIGUOUS_LINK_TYPE linkTo;
    public:
        Primary() : someClass(NULL), fieldTable(NULL), classMethod(NULL), interfaceMethod(NULL), paramTable(NULL),
                    localTable(NULL), ctorTable(NULL), linkTo(ALT_POSTPONED) {}
        virtual ~Primary() {}

        void setReferredClass(CompilationTable* set) { someClass = set; linkTo = ALT_TYPE; }
        void setReferredField(FieldTable* set) { fieldTable = set; linkTo = ALT_FIELD; }
        void setReferredClassMethod(ClassMethodTable* set) { classMethod = set; linkTo = ALT_CLASSMETHOD; }
        void setReferredInterfaceMethod(InterfaceMethodTable* set) { interfaceMethod = set; linkTo = ALT_INTERFACEMETHOD; }
        void setReferredParameter(ParamTable* set) { paramTable = set; linkTo = ALT_PARAM; }
        void setReferredLocalVar(LocalTable* set) { localTable = set; linkTo = ALT_LOCALVAR; }
        void setReferredConstructor(ConstructorTable* set) { ctorTable = set; linkTo = ALT_CTOR; }
        void setReferringToPrimitiveLiteral() { linkTo = ALT_PRIMITIVELIT; }
        void setReferringToArray() { linkTo = ALT_ARRAY; }
        // only possible to be set by FieldAccess
        void setReferringToArrayLength() { linkTo = ALT_ARRAYLENGTH; }
        void setReferringToNull() { linkTo = ALT_NULL; }
        void ResolveLinkButNoEntity() { linkTo = ALT_RESOLVEDNOENTITY; }

        CompilationTable* getReferredClass() { return someClass; }
        FieldTable* getReferredField() { return fieldTable; }
        ClassMethodTable* getReferredClassMethod() { return classMethod; }
        InterfaceMethodTable* getReferredInterfaceMethod() { return interfaceMethod; }
        ParamTable* getReferredParameter() { return paramTable; }
        LocalTable* getReferredLocalVar() { return localTable; }
        ConstructorTable* getReferredConstructor() { return ctorTable; }

        bool isReferringToClass() { return linkTo == ALT_TYPE; }
        bool isReferringToField() { return linkTo == ALT_FIELD; }
        bool isReferringToClassMethod() { return linkTo == ALT_CLASSMETHOD; }
        bool isReferringToInterfaceMethod() { return linkTo == ALT_INTERFACEMETHOD; }
        bool isReferringToParameter() { return linkTo == ALT_PARAM; }
        bool isReferringToLocalVar() { return linkTo == ALT_LOCALVAR; }
        bool isReferringToConstructor() { return linkTo == ALT_CTOR; }

        // ---------------------------------------------------------------------------
        // Miscellaneous linking

        // linking postponed? indicate something wrong
        bool postponeLinking() { return linkTo == ALT_POSTPONED; }
        // can only be set for LiteralOrThis
        bool linkToPrimitiveLiteral() { return linkTo == ALT_PRIMITIVELIT; }
        // can only be set for PrimaryNewArray
        bool linkToArray() { return linkTo == ALT_ARRAY; }
        // can only be set for field access
        bool linkToArrayLength() { return linkTo == ALT_ARRAYLENGTH; }
        // can only be set for LiteralOrThis
        bool linkToNull() { return linkTo == ALT_NULL; }
        bool resolvedLinkButNoEntity() { return linkTo == ALT_RESOLVEDNOENTITY; }
};

#endif
