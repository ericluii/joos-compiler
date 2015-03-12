#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include "ast.h"
#include "evaluatedType.h"

#include "compilationTable.h"
#include "typeString.h"

class Expression : public Ast {
    protected:
        EVALUATED_TYPE exprType;
        // in case it refers to some class or
        // interface or even an array of class/interface
        CompilationTable* typeTable;
    public:
        Expression() : exprType(ET_NOTEVALUATED), typeTable(NULL) {}
        virtual ~Expression() {}

        void setExprType(EVALUATED_TYPE set, CompilationTable* type = NULL) { exprType = set; typeTable = type; }
        // only call in the case that some readjusting need to be done
        // such as conversion from array to normal type vice versa
        void reconfigureExprType(EVALUATED_TYPE set) { exprType = set; }
        EVALUATED_TYPE getExprType() { return exprType; }
        CompilationTable* getTableTypeOfExpression() { return typeTable; }

        bool isExprTypeInt() { return exprType == ET_INT; }
        bool isExprTypeShort() { return exprType == ET_SHORT; }
        bool isExprTypeByte() { return exprType == ET_BYTE; }
        bool isExprTypeChar() { return exprType == ET_CHAR; }
        bool isExprTypeBoolean() { return exprType == ET_BOOLEAN; }
        bool isExprTypeIntArray() { return exprType == ET_INTARRAY; }
        bool isExprTypeShortArray() { return exprType == ET_SHORTARRAY; }
        bool isExprTypeByteArray() { return exprType == ET_BYTEARRAY; }
        bool isExprTypeCharArray() { return exprType == ET_CHARARRAY; }
        bool isExprTypeBooleanArray() { return exprType == ET_BOOLEANARRAY; }
        bool isExprTypeNull() { return exprType == ET_NULL; }
        bool isExprTypeObject() { return exprType == ET_OBJECT; }
        bool isExprTypeObjectArray() { return exprType == ET_OBJECTARRAY; }
        bool isExprTypeVoid() { return exprType == ET_VOID; }
        bool isExprTypeNotEvaluated() { return exprType == ET_NOTEVALUATED; }

        std::string getExpressionTypeString() {
            std::string ret = getCorrespondingTypeString(exprType, typeTable);
            if(isExprTypeObjectArray()) {
                ret+= "[]";
            }
            return ret;
        }
};

#endif
