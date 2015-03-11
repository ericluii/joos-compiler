#ifndef __NEWCLASSCREATION_H__
#define __NEWCLASSCREATION_H__

#include "primary.h"
#include "name.h"
#include "argumentsStar.h"

class CompilationTable;

class NewClassCreation : public Primary {
    // Rule: MAKE_NEW_CLASS
    private:
        Name* classType;
        ArgumentsStar* args;
        CompilationTable* classCreatedTable;
    public:
        NewClassCreation(Name* classType, ArgumentsStar* args) : Primary(), classType(classType), args(args),
                        classCreatedTable(NULL) {}
        ~NewClassCreation() {
            delete classType;
            delete args;
        }

        Name* getClassName() { return classType; }
        ArgumentsStar* getArgsToCreateClass() { return args; }
        void setTableOfCreatedClass(CompilationTable* set) { classCreatedTable = set; }
        CompilationTable* getTableOfCreatedClass() { return classCreatedTable; }

        std::string constructorInvocationMatchToSignature() {
            return "(" + args->stringifyArgumentsToType() + ")";
        }
};

#endif
