#ifndef __COMPILATIONTABLE_H__
#define __COMPILATIONTABLE_H__

#include <string>
#include <map>
#include <vector>
#include "packageDecl.h"
#include "symbolTable.h"

class CompilationUnit;
class ClassMethodTable;
class ConstructorTable;
class FieldTable;
class LocalTable;
class NestedBlockTable;
class ForTable;
class InterfaceMethodTable;
class ParamList;
class Identifier;
class Token;

class CompilationTable {
    private:
        PackageDecl* package;
        // NULL if no type is defined
        SymbolTable* symTable;
        std::string filename;
        CompilationUnit* unit;
        bool established;
        // other compilations in the same package
        std::vector<CompilationTable*>* compilationsInPackage;
        // compilations from single type import
        std::map<std::string, CompilationTable* > singleTypeImports;
        // compilations from import type on demand
        std::map<std::string, std::vector<CompilationTable*>* > importsOnDemand;
        // mappings for class methods and constructor
        std::map<std::string, ClassMethodTable*> classMethods;
        std::map<std::string, ConstructorTable*> constructors;
        std::map<std::string, FieldTable*> fields;
        // mappings for interface methods
        std::map<std::string, InterfaceMethodTable*> interfaceMethods;

        void reportLocalError(const std::string& conflict, const std::string& entity, Token* prevToken, Token* currToken);
        void registerFormalParameters(ParamList*, std::map<std::string, Token*>& localVars);
        void iterateThroughTable(SymbolTable* table, std::vector<std::map<std::string, Token*>* >& blockScopes);
        void checkBodyForOverlappingScope(SymbolTable* body, std::map<std::string, Token*>& localVars);
        
        void checkLocalTable(LocalTable& table, std::vector<std::map<std::string, Token*>* >& blockScopes);
        void checkNestedBlockTable(NestedBlockTable& table, std::vector<std::map<std::string, Token*>* >& blockScopes);
        void checkForTable(ForTable& table, std::vector<std::map<std::string, Token*>* >& blockScopes);

        void checkMethodForOverlappingScope(ClassMethodTable* methodTable);
        void checkConstructorForOverlappingScope(ConstructorTable* constructorTable);

        // Small helper during registering inherited fields and methods
        // Called from function inheritFieldsAndMethods
        void registerInheritedField(const std::string& field, FieldTable* table);
        void registerInheritedClassMethod(const std::string& methodSignature, ClassMethodTable* table);

    public:
        CompilationTable(PackageDecl* package, const std::string& filename, CompilationUnit* unit);
        ~CompilationTable();

        SymbolTable* getSymbolTable();
        std::string getPackageName();
        std::string getClassOrInterfaceName();
        std::string getCanonicalName();
        std::string getFilename();
        CompilationUnit* getCompilationUnit();
        void setSymbolTable(SymbolTable* set);
        void setCompilationsInPackage(std::vector<CompilationTable*>* tables);
        void setASingleTypeImport(const std::string& typeName, CompilationTable* table, Token* importTok);
        void setAnImportTypeOnDemand(const std::string& packageName, std::vector<CompilationTable*>* compilations);
        void checkForConflictingCanonicalName();

        // --------------------------------------------------------------------
        // Interface if symbol table is a class table
        FieldTable* getAField(const std::string& field);
        ClassMethodTable* getAClassMethod(const std::string& methodSignature);
        ConstructorTable* getAConstructor(const std::string& constructorSignature);
        void registerAField(const std::string& field, FieldTable* table);
        void registerClassMethodsAndConstructors();
        bool checkForFieldPresence(const std::string& field);
        bool checkForClassMethodPresence(const std::string& methodSignature);
        bool checkForConstructorPresence(const std::string& constructorSignature);

        // This part is to be called after hierarchy checking, involved with superclass extensions
        // Called to check if this compilation unit's inheritance have been properly resolved
        // i.e methods and fields have been properly inherited, applies only to compilations of classes
        bool isInheritanceEstablished();
        // Called after registering class methods, fields and constructors
        void inheritFieldsAndMethods();

        // ---------------------------------------------------------------------
        // Interface if symbol table is an interface table
        InterfaceMethodTable* getAnInterfaceMethod(const std::string& methodSignature);
        void registerInterfaceMethods();
        bool checkForInterfaceMethodPresence(const std::string& methodSignature);

        // ---------------------------------------------------------------------
        // Interface to check if symbol table is NULL or not
        bool aTypeWasDefined();

        // ---------------------------------------------------------------------
        // negation is isInterfaceSymbolTable
        bool isClassSymbolTable();

        // ---------------------------------------------------------------------
        // Symbol table analysis
        void checkForOverlappingLocalScope();

        // ---------------------------------------------------------------------
        // Interface to check presence of a type from import type on demand, 
        // single type import or other compilations in the package
        CompilationTable* checkTypePresenceFromSingleImport(const std::string& typeName);
        CompilationTable* checkTypePresenceInPackage(const std::string& typeName);
        CompilationTable* checkTypePresenceFromImportOnDemand(const std::string& typeName, Token* tokName);
};

#endif
