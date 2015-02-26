#ifndef __COMPILATIONTABLE_H__
#define __COMPILATIONTABLE_H__

#include <string>
#include <unordered_map>
#include "packageDecl.h"
#include "symbolTable.h"

class ClassMethodTable;
class ConstructorTable;
class FieldTable;
class InterfaceMethodTable;
class BuildCompilationTable;

class CompilationTable {
    friend class BuildCompilationTable;
    private:
        PackageDecl* package;
        SymbolTable* symTable;
        // mappings for class methods and constructor
        std::unordered_map<std::string, ClassMethodTable*> classMethods;
        std::unordered_map<std::string, ConstructorTable*> constructors;
        std::unordered_map<std::string, FieldTable*> fields;
        // mappings for interface methods
        std::unordered_map<std::string, InterfaceMethodTable*> interfaceMethods;
    public:
        CompilationTable(PackageDecl* package);
        ~CompilationTable();

        SymbolTable* getSymbolTable();
        std::string getPackageName();
        void setSymbolTable(SymbolTable* set);

        // --------------------------------------------------------------------
        // Interface if symbol table is a class table
        FieldTable* getAField(const std::string& field);
        ClassMethodTable* getAClassMethod(const std::string& methodSignature);
        ConstructorTable* getAConstructor(const std::string& constructorSignature);
        bool checkForFieldPresence(const std::string& field);
        bool checkForClassMethodPresence(const std::string& methodSignature);
        bool checkForConstructorPresence(const std::string& constructorSignature);

        // ---------------------------------------------------------------------
        // Interface if symbol table is an interface table
        InterfaceMethodTable* getAnInterfaceMethod(const std::string& methodSignature);
        bool checkForInterfaceMethodPresence(const std::string& methodSignature);

        // negation is isInterfaceSymbolTable
        bool isClassSymbolTable();
};

#endif
