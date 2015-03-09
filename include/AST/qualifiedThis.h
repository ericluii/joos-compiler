#ifndef __QUALIFIEDTHIS_H__
#define __QUALIFIEDTHIS_H__

#include "primary.h"
#include "name.h"

class CompilationTable;

class QualifiedThis : public Primary {
    private:
        Name* className;
        CompilationTable* qualifyingTable;
    public:
        QualifiedThis(Name* className) : Primary(), className(className), qualifyingTable(NULL) {}
        ~QualifiedThis() {
            delete className;
        }

        Name* getQualifyingClassName() { return className; }
        void setQualifyingClassTable(CompilationTable* set) { qualifyingTable = set; }
        CompilationTable* getQualifyingClassTable() { return qualifyingTable; }
};

#endif
