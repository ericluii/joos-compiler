#ifndef __FIELDDECL_H__
#define __FIELDDECL_H__

#include "classBodyDecls.h"
#include "type.h"
#include "identifier.h"
#include "expression.h"

class FieldDecl : public ClassBodyDecls {
    // Rule: CLASS_FIELD
    private:
        Type* fieldType;
        Identifier* fieldId;
        Expression* fieldExpression;
    public:
        FieldDecl(Modifiers* mod, Type* fieldType, Identifier* fieldId, Expression* fieldExpression) :
                  ClassBodyDecls(mod), fieldType(fieldType), fieldId(fieldId), fieldExpression(fieldExpression) {}
        ~FieldDecl() {
            delete fieldType;
            delete fieldId;
            delete fieldExpression;
        }

        Type* getFieldType() { return fieldType; }
        Identifier* getFieldDeclared() { return fieldId; }
        Expression* getInitializingExpression() { return fieldExpression; }

        bool isInitialized() { return fieldExpression != NULL; }
};

#endif
