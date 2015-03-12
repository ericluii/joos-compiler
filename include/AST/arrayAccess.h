#ifndef __ARRAYACCESS_H__
#define __ARRAYACCESS_H__

#include "ast.h"
#include "expression.h"

#include "evaluatedType.h"

class CompilationTable;

class ArrayAccess : public Primary {
    protected:
        Expression* accessExpr;
        EVALUATED_TYPE accessType;
        CompilationTable* accessedObject;
    public:
        ArrayAccess(Expression* accessExpr) : Primary(), accessExpr(accessExpr), accessType(ET_NOTEVALUATED), accessedObject(NULL) {}
        virtual ~ArrayAccess() {
            delete accessExpr;
        }
       
        void setTypeOfArrayElements(EVALUATED_TYPE set, CompilationTable* obj = NULL) { accessType = set; accessedObject = obj; }
        bool isAccessingIntArray() { return accessType == ET_INT; }
        bool isAccessingShortArray() { return accessType == ET_SHORT; }
        bool isAccessingByteArray() { return accessType == ET_BYTE; }
        bool isAccessingCharArray() { return accessType == ET_CHAR; }
        bool isAccessingBooleanArray() { return accessType == ET_BOOLEAN; }
        bool isAccessingObjectArray() { return accessType == ET_OBJECT; }
    
        EVALUATED_TYPE getTypeOfArrayElements() { return accessType; }
        CompilationTable* getTableOfArrayObjects() { return accessedObject; }

        Expression* getAccessExpression() { return accessExpr; }
};

#endif
