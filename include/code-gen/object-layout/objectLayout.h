#ifndef __OBJECTLAYOUT_H__
#define __OBJECTLAYOUT_H__

class CompilationTable;
class FieldTable;

#include <vector>
#include <map>

class ObjectLayout {
    // Layout of an object
    /*****************************
     *    length (if an array)   *
     *---------------------------*
     *     virtual table         *
     *---------------------------*
     *     inheritance table     *
     *---------------------------*
     *   interface method table  *
     *---------------------------*
     *     non-static members    *
     *                           *
     * --------------------------*/

    private:
        // fields declared in the class, in the order they were declared
        // with superclass's fields coming first, only non-static ones
        
        // superclass layout
        ObjectLayout* parentLayout;
        // the size of tables in bytes
        static unsigned int sizeOfTables;
        std::vector<FieldTable*> declaredFields;

        void createLayout(ObjectLayout* parentLayout, CompilationTable*);
    public:
        ObjectLayout(ObjectLayout*, CompilationTable*);

        unsigned int sizeOfObject();
        unsigned int indexOfFieldInObject(FieldTable*);
        unsigned int numberOfFieldsInObject();
        static unsigned int transformToFieldIndexInAClass(unsigned int);
};

#endif
