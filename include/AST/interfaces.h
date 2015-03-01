#ifndef __INTERFACES_H__
#define __INTERFACES_H__

#include "ast.h"
#include "name.h"

class Interfaces : public Ast {
    // Rule: INTERFACE_TYPE_LIST_END, INTERFACE_TYPE_LIST, EXTENDING and EXTENDING_LIST
    private:
        Name* interface;
        Interfaces* nextInterface;
    public:
        Interfaces(Name* interface) : interface(interface), nextInterface(NULL) {}
        ~Interfaces() {
            delete interface;
            delete nextInterface;
        }

        Name* getCurrentInterface() { return interface; }
        Interfaces* getNextInterface() { return nextInterface; }
        void setNextInterface(Interfaces* set) { nextInterface = set; }
        bool isLastInterface() { return nextInterface == NULL; }
};

#endif
