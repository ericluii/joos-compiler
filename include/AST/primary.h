#ifndef __PRIMARY_H__
#define __PRIMARY_H__

#include "ast.h"
#include "ambiguousLinkType.h"

class CompilationTable;
class FieldTable;
class ClassMethodTable;
class InterfaceMethodTable;
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
        ConstructorTable* ctorTable; // NewClassCreation

        AMBIGUOUS_LINK_TYPE linkTo;
    public:
        Primary() : someClass(NULL), fieldTable(NULL), classMethod(NULL), interfaceMethod(NULL),
                    ctorTable(NULL), linkTo(ALT_POSTPONED) {}
        virtual ~Primary() {}

        void setReferredClass(CompilationTable* set) { someClass = set; linkTo = ALT_TYPE; }
        void setReferredField(FieldTable* set) { fieldTable = set; linkTo = ALT_FIELD; }
        void setReferredClassMethod(ClassMethodTable* set) { classMethod = set; linkTo = ALT_CLASSMETHOD; }
        void setReferredInterfaceMethod(InterfaceMethodTable* set) { interfaceMethod = set; linkTo = ALT_INTERFACEMETHOD; }
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
        ConstructorTable* getReferredConstructor() { return ctorTable; }

        bool isReferringToClass() { return linkTo == ALT_TYPE; }
        bool isReferringToField() { return linkTo == ALT_FIELD; }
        bool isReferringToClassMethod() { return linkTo == ALT_CLASSMETHOD; }
        bool isReferringToInterfaceMethod() { return linkTo == ALT_INTERFACEMETHOD; }
        bool isReferringToConstructor() { return linkTo == ALT_CTOR; }

        // ---------------------------------------------------------------------------
        // Miscellaneous linking

        // linking postponed? indicate something went wrong previously
        // propagate errors? up to you, probably not though
        bool postponeLinking() { return linkTo == ALT_POSTPONED; }
        // can only be set for LiteralOrThis in the case of
        // number, char, true and false literals
        bool linkToPrimitiveLiteral() { return linkTo == ALT_PRIMITIVELIT; }
        // can only be set for PrimaryNewArray
        bool linkToArray() { return linkTo == ALT_ARRAY; }
        // can only be set for FieldAccess,
        // refers to the static field of the length of an array
        bool linkToArrayLength() { return linkTo == ALT_ARRAYLENGTH; }
        // can only be set for LiteralOrThis in the case of null literal
        bool linkToNull() { return linkTo == ALT_NULL; }
        // used for:
        // BracketedExpression, ArrayAccess (both Name and Primary) -> don't really link to anything
        // QualifiedThis -> resolved already in A2
        // manual checking needs to be done for these entities
        bool resolvedLinkButNoEntity() { return linkTo == ALT_RESOLVEDNOENTITY; }

        AMBIGUOUS_LINK_TYPE getLinkType() { return linkTo; }
};

#endif
