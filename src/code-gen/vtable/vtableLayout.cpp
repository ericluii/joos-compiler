#include <cassert>
#include <set>

#include "vtableLayout.h"
#include "compilationTable.h"
#include "classMethodTable.h"
#include "interfaceMethodTable.h"
#include "classMethod.h"
#include "interfaceMethod.h"

// Label
#include "labelManager.h"

VTableLayout::VTableLayout(CompilationTable* table, VTableLayout* superVTable) : typeName(table->getCanonicalName()),
            superclassVTable(superVTable) {
    // invoked for reference types
    createVTable(table);
}

VTableLayout::VTableLayout(VTableLayout* superVTable) : typeName(LabelManager::labelizeForArrays("")),
             superclassVTable(superVTable) {
    // invoked for array type
    createVTableForArray();
}

void VTableLayout::createVTableForArray() {
    // should only be called for array types
    for(unsigned int i = 0; i != superclassVTable->virtualMethods.size(); i++) {
        SymbolTable* method = superclassVTable->virtualMethods[i];
        // the superclass of an array is java.lang.Object, which only
        // has class methods
        assert(method->isClassMethodTable());
        std::string methodSignature = ((ClassMethodTable*) method)->getClassMethod()->getMethodHeader()->methodSignatureAsString();
        virtualMethods.push_back(method);
        virtualMethodsMapping[methodSignature] = i;
    }
}

void VTableLayout::createVTable(CompilationTable* table) {
    std::set<SymbolTable*> registeredMethods;
    unsigned int numVirtualMethods = 0;
    if(superclassVTable != NULL) {
        // not NULL, if NULL then the class must be java.lang.Object
        for(; numVirtualMethods != superclassVTable->virtualMethods.size(); numVirtualMethods++) {
            // for each method, copy over the virtual methods of it's superclass, override
            // those that are overridden in this class
            SymbolTable* method = superclassVTable->virtualMethods[numVirtualMethods];

            std::string methodSignature;
            if(method->isClassMethodTable()) {
                // if we get here then the method is a class method
                methodSignature = ((ClassMethodTable*) method)->getClassMethod()->getMethodHeader()->methodSignatureAsString();
                if(table->classMethodIsInherited(methodSignature)) {
                    // class method is inherited, then copy whatever is in the 
                    // vtable of the superclass
                    virtualMethods.push_back(method);
                    // assign to mapping
                    virtualMethodsMapping[methodSignature] = numVirtualMethods;
                } else {
                    // class method is not inherited
                    // this class method will be from this class's
                    method = table->getAClassMethod(methodSignature);
                    // precautionary check
                    assert(method != NULL);
                    virtualMethods.push_back(method);
                    virtualMethodsMapping[methodSignature] = numVirtualMethods;
                    // indicate that the method has been checked
                    registeredMethods.insert(method);
                }
            } else {
                // method is an interface method table
                methodSignature = ((InterfaceMethodTable*) method)->getInterfaceMethod()->methodSignatureAsString();
                SymbolTable* declaredClassMethod = table->getAClassMethod(methodSignature);
                if(declaredClassMethod != NULL) {
                    virtualMethods.push_back(declaredClassMethod);
                    registeredMethods.insert(declaredClassMethod);
                } else {
                    // the method could not be found in the class as a
                    // class method, then push back the interface method from the
                    // superclass vtable, which will be what the current class have
                    // anyway by the way we inherit them (refer to compilationTable.cpp)
                    virtualMethods.push_back(method);
                    registeredMethods.insert(method);
                }
                virtualMethodsMapping[methodSignature] = numVirtualMethods;
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
            virtualMethodsMapping[it->second->getClassMethod()->getMethodHeader()->methodSignatureAsString()] = numVirtualMethods++;
        }
    }

    std::map<std::string, InterfaceMethodTable*>& inheritedInterfaceMethods = table->getInheritedInterfaceMethodsForClass();
    std::map<std::string, InterfaceMethodTable*>::iterator it2;
    for(it2 = inheritedInterfaceMethods.begin(); it2 != inheritedInterfaceMethods.end(); it2++) {
        if(registeredMethods.count(it2->second) == 0) {
            virtualMethods.push_back(it2->second);
            virtualMethodsMapping[it2->second->getInterfaceMethod()->methodSignatureAsString()] = numVirtualMethods++;
        }
    }
}

unsigned int VTableLayout::getIndexOfMethodInVTable(const std::string& methodSignature) {
    if(virtualMethodsMapping.count(methodSignature) == 1) {
        return virtualMethodsMapping[methodSignature];
    }
    // if we reach here, then indicates some form of error
    assert(false);
}

void VTableLayout::outputVTableToFile(std::ofstream& file) {
    std::string virtualTableName = getVirtualTableName();
    file << "global " + virtualTableName + "\n";
    file << virtualTableName + ":\n";
    for(unsigned int i = 0; i < virtualMethods.size(); i++) {
        SymbolTable* classMethod = virtualMethods[i];
        // precautionary check, this method should only be
        // called for non-abstract classes, which do not inherit
        // any interface methods then
        assert(classMethod->isClassMethodTable());
        std::string methodLabel = ((ClassMethodTable*) virtualMethods[i])->generateMethodLabel();
        file << "extern " << methodLabel << "\n";
        file << "dd " << methodLabel << "\n";
    }
    file << std::endl;
}

std::string VTableLayout::getVirtualTableName() {
    return LabelManager::labelizeToVirtualTable(typeName);
}
