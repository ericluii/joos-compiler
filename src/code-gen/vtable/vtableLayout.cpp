#include <cassert>
#include <set>

#include "vtableLayout.h"
#include "compilationTable.h"
#include "classMethod.h"

VTableLayout::VTableLayout(CompilationTable* table, VTableLayout* superVTable) : typeName(table->getCanonicalName()),
            superclassVTable(superVTable) {
    // invoked for reference types
    createVTable(table);
}

VTableLayout::VTableLayout(VTableLayout* superVTable) : typeName(".array"), superclassVTable(superVTable) {
    // invoked for array type
    createVTableForArray();
}

void VTableLayout::createVTableForArray() {
    // should only be called for array types
    for(unsigned int i = 0; i != superclassVTable->virtualMethods.size(); i++) {
        ClassMethodTable* classMethod = superclassVTable->virtualMethods[i];
        virtualMethods.push_back(classMethod);
        virtualMethodsMapping[classMethod] = i;
    }
}

void VTableLayout::createVTable(CompilationTable* table) {
    std::set<ClassMethodTable*> registeredMethods;
    unsigned int numVirtualMethods = 0;
    if(superclassVTable != NULL) {
        // not NULL, if NULL then the class must be java.lang.Object
        for(; numVirtualMethods != superclassVTable->virtualMethods.size(); numVirtualMethods++) {
            // for each method, copy over the virtual methods of it's superclass, override
            // those that are overridden in this class
            ClassMethodTable* classMethod = superclassVTable->virtualMethods[numVirtualMethods];
            std::string methodSignature = classMethod->getClassMethod()->getMethodHeader()->methodSignatureAsString();
            if(table->classMethodIsInherited(methodSignature)) {
                // class method is inherited, then copy whatever is in the 
                // vtable of the superclass
                virtualMethods.push_back(classMethod);
                // assign to mapping
                virtualMethodsMapping[classMethod] = numVirtualMethods;
            } else {
                // class method is not inherited
                // this class method will be from this class's
                classMethod = table->getAClassMethod(methodSignature);
                // precautionary check
                assert(classMethod != NULL);
                virtualMethods.push_back(classMethod);
                virtualMethodsMapping[classMethod] = numVirtualMethods;
                // indicate that the method has been checked
                registeredMethods.insert(classMethod);
            }
        }
    }

    // get number of virtual methods currently held
    numVirtualMethods = virtualMethods.size();

    // now register the rest of the methods defined in the class
    std::map<std::string, ClassMethodTable*>& definedClassMethods = table->getDefinedClassMethods();
    std::map<std::string, ClassMethodTable*>::iterator it;
    for(it = definedClassMethods.begin(); it != definedClassMethods.end(); it++) {
        if(registeredMethods.count(it->second) == 0 && !it->second->getClassMethod()->isStatic()) {
            // if method has not been registered to vtable and it is not
            // static, above the static check is not done because static methods
            // cannot override non-static ones
            virtualMethods.push_back(it->second);
            // assign to mapping then increment
            virtualMethodsMapping[it->second] = numVirtualMethods++;
        }
    }
}

unsigned int VTableLayout::getIndexOfMethodInVTable(ClassMethodTable* method) {
    if(virtualMethodsMapping.count(method) == 1) {
        return virtualMethodsMapping[method];
    }
    // if we reach here, then indicates some form of error
    assert(false);
}

void VTableLayout::outputVTableToFile(std::fstream& file) {
    // TODO: Later
    std::string virtualTableName = getVirtualTableName();
    file << "global " + virtualTableName + "\n";
    file << virtualTableName + ":\n";
    for(unsigned int i = 0; i < virtualMethods.size(); i++) {
        std::string methodLabel = virtualMethods[i]->generateMethodLabel();
        file << "extern " << methodLabel << "\n";
        file << "dd " << methodLabel << "\n";
    }
    file << std::endl;
}

std::string VTableLayout::getVirtualTableName() {
    return "VIRT"+typeName;
}
