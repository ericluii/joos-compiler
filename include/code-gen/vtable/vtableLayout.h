#ifndef __VTABLE_LAYOUT_H__
#define __VTABLE_LAYOUT_H__

#include <map>
#include <vector>
#include <string>
#include <set>
#include <fstream>

class CompilationTable;
class ClassMethodTable;

class VTableLayout {
    // each VTableLayout represents the layout
    // of the vtable of a class
    private:
        // refers to the length of the typeName string
        std::string typeName;
        VTableLayout* superclassVTable;
        // vector of methods that a class have, inherited or
        // defined in the class, order of methods is based on
        // the order of the methods in the vtable of the superclass
        // first then followed by the current class
        //
        // methods of the superclass which are overridden by the
        // current class will have the method overridden by the
        // method of the current class
        //
        // this vector will not include abstract and static methods
        // because:
        // - Static methods will be treated as global methods and thus
        // do not need to be in the virtual table. This does not cause
        // conflict since static methods can't override instance methods
        // and vice versa
        std::vector<ClassMethodTable*> virtualMethods;
        // mapping of various methods to its index in the virtual
        // table of the class that the method is defined in
        std::map<ClassMethodTable*, unsigned int> virtualMethodsMapping;

        void createVTableForArray();
        void createVTable(CompilationTable* table);
    public:
        VTableLayout(CompilationTable*, VTableLayout*);
        VTableLayout(VTableLayout*);
        unsigned int getIndexOfMethodInVTable(ClassMethodTable*);
        void outputVTableToFile(std::ofstream&);
        std::string getVirtualTableName();
};

#endif
