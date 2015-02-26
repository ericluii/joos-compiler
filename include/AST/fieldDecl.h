#ifndef __FIELDDECL_H__
#define __FIELDDECL_H__

#include "classBodyDecls.h"
#include "type.h"
#include "identifier.h"
#include "expression.h"
#include "fieldTable.h"

class FieldDecl : public ClassBodyDecls {
    // Rule: CLASS_FIELD
    private:
        Type* fieldType;
        Identifier* fieldId;
        Expression* fieldExpression;
        FieldTable* table;
    public:
        FieldDecl(Modifiers* mod, Type* fieldType, Identifier* fieldId, Expression* fieldExpression) :
                  ClassBodyDecls(mod), fieldType(fieldType), fieldId(fieldId), fieldExpression(fieldExpression), table(NULL) {}
        ~FieldDecl() {
            delete fieldType;
            delete fieldId;
            delete fieldExpression;
        }

        Type* getFieldType() { return fieldType; }
        Identifier* getFieldDeclared() { return fieldId; }
        Expression* getInitializingExpression() { return fieldExpression; }

        bool isInitialized() { return fieldExpression != NULL; }
        void setFieldTable(FieldTable* set) { table = set; }
        FieldTable* getFieldTable() { return table; }
};

#endif
