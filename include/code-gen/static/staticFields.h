#ifndef __STATICFIELDS_H__
#define __STATICFIELDS_H__

#include <vector>

class FieldTable;
class CompilationTable;

class StaticFields {
    private:
        // contain static fields only, in the order
        // in which they are declared in their declaring class
        std::vector<FieldTable*> statics;
    public:
        StaticFields(CompilationTable*);
        std::vector<FieldTable*>& getAllStaticFieldsOfClass(); 
};

#endif
