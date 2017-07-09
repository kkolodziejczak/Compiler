#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <iostream>
#include <vector>
#include <ctype.h>

#include "Utils.hpp"
#include "Manager.hpp"
#include "VariableInfo.hpp"

class Generator{
    private:
        std::vector<std::string> ProgramCode;
        Manager *manager;
        std::string space;
        Utils util;
    public:
        Generator(Manager *m){
            space = "\t\t";
            ProgramCode.push_back(".text\n");
            ProgramCode.push_back(space + "b main\n\n");
            manager = m;
        };
//=========================== Utils =================================

        std::vector<std::string> generateCodeForVariables();
        void writeCode(std::string code, bool applaySpace = true);
        std::vector<std::string> getProgramCode();

//============================ I/O ==================================

        void generateCodeForPrint(bool newLine);
        void generateCodeForInput(std::string name);

//======================== Expression ===============================

        void generateCodeForExpression(char ExprSymbol);
        void generateCodeForExpressionSymbol(std::string command, VariableInfo var1, VariableInfo var2);
        void generateCodeForModulo();

//========================== Arrays =================================

        void generateCodeForArrayAssignment();
        std::string getArrayPosition(VariableInfo var);
        bool isArray(std::string variableName);

//============================ IF ===================================

        void generateCodeForIfStatement(LogicExpr expr, bool whileStatement = false);
        std::string generateCodeForJump();
        void generateJumpToLabel();
        void generateCodeForWhileEnd();

//========================= Function ================================

        void generateCodeForFunctionJump(std::string functionName);
        void generateCodeForFunction(std::string functionName);
        void generateCodeForFunctionJumpOut(std::string functionName);
        void generateCodeForReturnValue();
};

#endif