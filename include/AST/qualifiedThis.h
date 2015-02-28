#ifndef __QUALIFIEDTHIS_H__
#define __QUALIFIEDTHIS_H__

#include "primary.h"
#include "name.h"

class QualifiedThis : public Primary {
    private:
        Name* className;
    public:
        QualifiedThis(Name* className) : className(className) {}
        ~QualifiedThis() {
            delete className;
        }

        Name* getQualifyingClassName() { return className; }
};

#endif
