#include <cassert>

#include "vtableLayout.h"
#include "compilationTable.h"
#include "classMethodTable.h"
#include "interfaceMethodTable.h"
#include "classMethod.h"
#include "interfaceMethod.h"

// Label
#include "labelManager.h"

VTableLayout::VTableLayout(CompilationTable* table, VTableLayout* superVTable) : type(table),
    typeName(table->getCanonicalName()), superclassVTable(superVTable) {
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
    unsigned int numVirtualMethods = 0;
    if(superclassVTable != NULL) {
        // not NULL, if NULL then the class must be java.lang.Object
        for(; numVirtualMethods < superclassVTable->virtualMethods.size(); numVirtualMethods++) {
            // for each method, copy over the virtual methods of it's superclass, override
            // those that are overridden in this class
            SymbolTable* method = superclassVTable->virtualMethods[numVirtualMethods];
            std::string methodSignature;

            if(method->isClassMethodTable()) {
                // if we get here then the method is a class method
                methodSignature = ((ClassMethodTable*) method)->getClassMethod()->getMethodHeader()->methodSignatureAsString();
                virtualMethods.push_back(method);
                // assign to mapping
                virtualMethodsMapping[methodSignature] = numVirtualMethods;
            } else {
                // method is an interface method table
                methodSignature = ((InterfaceMethodTable*) method)->getInterfaceMethod()->methodSignatureAsString();
                virtualMethods.push_back(method);
                virtualMethodsMapping[methodSignature] = numVirtualMethods;
            }
        }
    }

    // now register the rest of the methods defined in the class
    std::map<std::string, ClassMethodTable*>& definedClassMethods = table->getDefinedClassMethods();
    std::map<std::string, ClassMethodTable*>::iterator it;
    for(it = definedClassMethods.begin(); it != definedClassMethods.end(); it++) {
        if(!it->second->getClassMethod()->isStatic()) {
            // if method has not been registered to vtable and it is not
            // static, above the static check is not done because static methods
            // cannot override non-static ones
            std::string methodSignature = it->second->getClassMethod()->getMethodHeader()->methodSignatureAsString();
            if(virtualMethodsMapping.count(methodSignature) == 1) {
                // superclass has a method with the same method signature
                // then do a simple replacement
                virtualMethods[virtualMethodsMapping[methodSignature]] = it->second;
            } else {
                // superclass does not have a method with this signature
                virtualMethods.push_back(it->second);
                // assign then increment
                virtualMethodsMapping[methodSignature] = numVirtualMethods++;
            }
        }
    }

    std::map<std::string, InterfaceMethodTable*>& inheritedInterfaceMethods = table->getInheritedInterfaceMethodsForClass();
    std::map<std::string, InterfaceMethodTable*>::iterator it2;
    for(it2 = inheritedInterfaceMethods.begin(); it2 != inheritedInterfaceMethods.end(); it2++) {
        std::string methodSignature = it2->second->getInterfaceMethod()->methodSignatureAsString();
        if(virtualMethodsMapping.count(methodSignature) == 1) {
            virtualMethods[virtualMethodsMapping[methodSignature]] = it2->second;
        } else {
            virtualMethods.push_back(it->second);
            virtualMethodsMapping[methodSignature] = numVirtualMethods++;
        }
    }
}

unsigned int VTableLayout::getIndexOfMethodInVTable(const std::string& methodSignature) {
    if(virtualMethodsMapping.count(methodSignature) == 1) {
        // multiplied by 4 since every entry is 4 bytes
        return virtualMethodsMapping[methodSignature] * 4;
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
        if(((ClassMethodTable*) virtualMethods[i])->getDeclaringClass() != type) {
            // if the method is not the method of the type
            // of the virtual table, then we need to extern
            file << "extern " << methodLabel << "\n";
        }
        file << "dd " << methodLabel << "\n";
    }
    file << std::endl;
}

std::string VTableLayout::getVirtualTableName() {
    return LabelManager::labelizeToVirtualTable(typeName);
}
