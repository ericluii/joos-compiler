#ifndef __NEWCLASSCREATION_H__
#define __NEWCLASSCREATION_H__

#include "primary.h"
#include "name.h"
#include "argumentsStar.h"

class NewClassCreation : public Primary {
    private:
        Name* classType;
        ArgumentsStar* args;
    public:
        NewClassCreation(Name* classType, ArgumentsStar* args) : classType(classType), args(args) {}
        ~NewClassCreation() {
            delete classType;
            delete args;
        }

        Name* getClassType() { return classType; }
        ArgumentsStar* getArgsStar() { return args; }
};

#endif
