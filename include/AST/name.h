#ifndef __NAMEDECL_H__
#define __NAMEDECL_H__

#include "ast.h"
#include "identifier.h"
#include "ambiguousLinkType.h"

class CompilationTable;
class FieldTable;
class LocalTable;
class ParamTable;
class Package;

class Name : public Ast
{
    // Rule: NAME_SIMPLE and NAME_QUALIFIED
    private:
        Name *nextName;
        Identifier *id;

        // a name is either referring to a class, a field, a local variable, a parameter,
        // a class method or a package. It is not possible to be referring to an interface since
        // it has no meaning in Joos, unlike in Java which has interface constants
        // only one will be set at any time
        CompilationTable* someClass;
        FieldTable* fieldTable;
        ParamTable* paramTable;
        LocalTable* localTable;
        Package* pkg;
        
        AMBIGUOUS_LINK_TYPE linkTo;
    public:
        Name(Identifier *id): nextName(NULL), id(id), someClass(NULL), fieldTable(NULL),
                    paramTable(NULL), localTable(NULL), pkg(NULL), linkTo(ALT_POSTPONED) {}
        ~Name() {
            delete id;
            delete nextName;
        }

        Identifier *getNameId() { return id; }
        Name *getNextName() { return nextName; }
        std::string getQualifier() {
            if(nextName == NULL) {
                return "";
            }
            return nextName->getFullName();
        }

        std::string getFullName() {
            if(nextName == NULL) {
                return id->getIdAsString();
            }
            return nextName->getFullName() + "." + id->getIdAsString();
        }

        void setNextName(Name* set) { nextName = set; }
        bool isLastPrefix() { return nextName == NULL; }

        // ----------------------------------------------------------------
        // Called during disambiguation stage
        
        void setReferredClass(CompilationTable* set) { someClass = set; linkTo = ALT_CLASS; }
        void setReferredField(FieldTable* set) { fieldTable = set; linkTo = ALT_FIELD; }
        void setReferredParameter(ParamTable* set) { paramTable = set; linkTo = ALT_PARAM; }
        void setReferredLocalVar(LocalTable* set) { localTable = set; linkTo = ALT_LOCALVAR; }
        void setReferredPackage(Package* set) { pkg = set; linkTo = ALT_PACKAGE; }
        void setReferringToArrayLength() { linkTo = ALT_ARRAYLENGTH; }

        // the assumption is that only one will be true at any time
        bool isReferringToClass() { return linkTo == ALT_CLASS; }
        bool isReferringToField() { return linkTo == ALT_FIELD; }
        bool isReferringToParameter() { return linkTo == ALT_PARAM; }
        bool isReferringToLocalVar() { return linkTo == ALT_LOCALVAR; }
        bool isReferringToPackage() { return linkTo == ALT_PACKAGE; }

        CompilationTable* getReferredClass() { return someClass; }
        FieldTable* getReferredField() { return fieldTable; }
        ParamTable* getReferredParameter() { return paramTable; }
        LocalTable* getReferredLocalVar() { return localTable; }
        Package* getReferredPackage() { return pkg; }

        // used to check if no linking has been done
        bool postponeLinking() { return linkTo == ALT_POSTPONED; }
        bool linkToArraylength() { return linkTo == ALT_ARRAYLENGTH; }
};

#endif
