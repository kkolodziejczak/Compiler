#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <map>
#include <stack>
#include <vector>
#include <sstream>
#include <iostream>

#include "Utils.hpp"
#include "VariableInfo.hpp"

class Manager{
    private:
        std::stack<VariableInfo> VariableStack;
        std::stack<std::string> LabelStack;
        std::map<std::string, VariableInfo> SymbolTable;
        std::map<std::string, Type> FunctionTable;
        int StringIterator;
        int ExpressionIterator;
        int LabelIterator;
        Utils util;
    public:
        Manager(){
            StringIterator = 0;
            ExpressionIterator = 0;
            LabelIterator = 0;
        };
//=========================== Utils =================================

        std::map<std::string, VariableInfo> getSymbolTable();
        VariableInfo checkIfSymbolExist(std::string name);

//========================= Variable ================================

        void registerVariable(std::string name,
            Type type, std::string value, std::vector<std::string> dim,
            std::vector<int> size, int arrSize);
        void registerVariable(std::string name, Type type, 
                                std::string value);
        void registerVariable(std::string name, Type type);
        void registerVariable(VariableInfo variable);
        void registerVariable();
        void registerString(Type type, std::string value);
        VariableInfo getItemFromVariableStack();
        void pushVariableOnStack(VariableInfo variable);
        void pushVariableOnStack(std::string name, Type type);
        void pushVariableOnStack(std::string name, Type type, 
                                    std::string value);
        void pushExistingVariable(std::string name);

//======================== Expression ===============================

        std::string registerExpresionValue(Type type);

//========================== Label ==================================

        std::string registerLabel();
        bool isLabelStackEmpty();
        int getLabelStackSize();
        void pushLabelOnStack(std::string label);
        std::string getItemFromLabelStack();
        
//========================= Function ================================

        std::map<std::string, Type> getFunctionTable();
        bool isFunctionDeclared(std::string name);
        void registerFunction(std::string functionName, Type type);
};

#endif