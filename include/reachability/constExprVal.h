#ifndef __CONSTEXPRVAL_H__
#define __CONSTEXPRVAL_H__

enum CONST_EXPR_VAL {
    // indicates that a constant expression
    // evaluates to true
    CE_TRUE,
    // indicates that a constant expression
    // evaluates to false
    CE_FALSE,
    // indicates that an expression is representing a
    // literal suited for a constant expression,
    // i.e either a literal of a primitive
    // type or a string literal
    CE_PRIMITIVEORSTRING,
    // indicates that expression is actually not
    // a constant expression, based on the definition
    // in JLS 15.28
    CE_NONCONSTEXPR,
};

#endif
