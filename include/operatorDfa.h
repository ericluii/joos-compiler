#ifndef __OPERATORDFA_H__
#define __OPERATORDFA_H__

#include "dfa.h"
#include <string>

class OperatorDfa final : public Dfa {
    // the kind of operators which are valid in joos 1W are:
    // - those that involve arithmetic operations, (+,-,*,/,%)
    // - those that involve comparison operations, (<, >, <=, >=, ==, !=)
    // - those that involve eager boolean operations, (&, |, ^) -> but no bitwise operations
    // - those that involve lazy boolean operations, (&&, ||)
    // - instanceof
    // - casting (both reference and primitive)
    //
    // Hence there are no:
    // +=, -=, &=, |=, ^= etc., ? ternary operator, the unary plus operator, ++ or --, bit shifts
    // bitwise operations (remember that &,|,^ are only for the eager boolean operation)
    private:
        // needed to check for lazy/eager boolean op

        void initDfa();
        TOKEN_TYPE getTokenType();
    public:
        OperatorDfa();

        static TOKEN_TYPE endTokenType;
        static char previousChar;
        static std::string operators;
};

#endif
