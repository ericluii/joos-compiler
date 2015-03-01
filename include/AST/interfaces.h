#ifndef __INTERFACES_H__
#define __INTERFACES_H__

#include "ast.h"
#include "name.h"

class CompilationTable;

class Interfaces : public Ast {
    // Rule: INTERFACE_TYPE_LIST_END, INTERFACE_TYPE_LIST, EXTENDING and EXTENDING_LIST
    private:
        Name* interface;
        Interfaces* nextInterface;
        CompilationTable* implOrExtTable;
    public:
        Interfaces(Name* interface) : interface(interface), nextInterface(NULL), implOrExtTable(NULL) {}
        ~Interfaces() {
            delete interface;
            delete nextInterface;
        }

        Name* getCurrentInterface() { return interface; }
        Interfaces* getNextInterface() { return nextInterface; }
        void setImplOrExtInterfaceTable(CompilationTable* set) { implOrExtTable = set; }
        CompilationTable* getImplOrExtInterfaceTable() { return implOrExtTable; }
        void setNextInterface(Interfaces* set) { nextInterface = set; }
        bool isLastInterface() { return nextInterface == NULL; }
};

#endif
