#ifndef __INTERFACES_H__
#define __INTERFACES_H__

#include "ast.h"
#include "name.h"

class Interfaces : public Ast {
    // Rule: INTERFACE_TYPE_LIST_END and INTERFACE_TYPE_LIST
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
        bool lastInterface() { return nextInterface == NULL; }
};

#endif
