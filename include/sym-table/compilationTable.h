#ifndef __COMPILATIONTABLE_H__
#define __COMPILATIONTABLE_H__

#include <string>
#include <map>
#include <vector>
#include "packageDecl.h"
#include "symbolTable.h"

class ClassMethodTable;
class ConstructorTable;
class FieldTable;
class LocalTable;
class NestedBlockTable;
class ForTable;
class InterfaceMethodTable;
class BuildCompilationTable;
class ParamList;
class Token;

class CompilationTable {
    friend class BuildCompilationTable;
    private:
        PackageDecl* package;
        SymbolTable* symTable;
        // mappings for class methods and constructor
        std::map<std::string, ClassMethodTable*> classMethods;
        std::map<std::string, ConstructorTable*> constructors;
        std::map<std::string, FieldTable*> fields;
        // mappings for interface methods
        std::map<std::string, InterfaceMethodTable*> interfaceMethods;

        void reportError(const std::string& conflict, const std::string& entity, Token* prevToken, Token* currToken);
        void registerFormalParameters(ParamList*, std::map<std::string, Token*>& localVars);
        void iterateThroughTable(SymbolTable* table, std::vector<std::map<std::string, Token*>* >& blockScopes);
        void checkBodyForOverlappingScope(SymbolTable* body, std::map<std::string, Token*>& localVars);
        
        void checkLocalTable(LocalTable& table, std::vector<std::map<std::string, Token*>* >& blockScopes);
        void checkNestedBlockTable(NestedBlockTable& table, std::vector<std::map<std::string, Token*>* >& blockScopes);
        void checkForTable(ForTable& table, std::vector<std::map<std::string, Token*>* >& blockScopes);

        void checkMethodForOverlappingScope(ClassMethodTable* methodTable);
        // void checkConstructorForOverlappingScope(ConstructorTable* constructorTable);
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

        // ---------------------------------------------------------------------
        // negation is isInterfaceSymbolTable
        bool isClassSymbolTable();

        // ---------------------------------------------------------------------
        // Symbol table analysis
        void checkForOverlappingLocalScope();
};

#endif
