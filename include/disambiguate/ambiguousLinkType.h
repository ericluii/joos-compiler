#ifndef __AMBIGUOUSLINKTYPE_H__
#define __AMBIGUOUSLINKTYPE_H__

enum AMBIGUOUS_LINK_TYPE {
    ALT_POSTPONED = 0,
    ALT_FIELD,
    ALT_LOCALVAR,
    ALT_PARAM,
    ALT_CLASSMETHOD,
    ALT_INTERFACEMETHOD,
    ALT_CLASS,
    ALT_CTOR,
    ALT_PACKAGE,
    // this link is to some primtive literal
    // i.e a char, true, false, an int
    // Only possible to be assigned in LiteralOrThis
    ALT_PRIMITIVELIT,
    // indicates linked to an array of some type
    // Only possible to be assigned in PrimaryNewArray
    ALT_ARRAY,
    // indicates that it links to the length of an
    // array, it's type is int
    ALT_ARRAYLENGTH,
    // pointing to NULL, only possible to be set in LiteralOrThis
    ALT_NULL,
    // pointing to an expression, special case for BracketedExpression
    ALT_EXPR,
    // indicates that linking has been resolved but no link entity
    // was made, manually check in these cases.
    // Applied in:
    // - QualifiedThis, which has already been checked in A2
    // - BracketedExpression, which does not link to anything
    ALT_RESOLVEDNOENTITY,
};

#endif
