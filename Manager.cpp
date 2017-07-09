#include "Manager.hpp"

//=========================== Utils =================================

std::map<std::string, VariableInfo> Manager::getSymbolTable(){
    return SymbolTable;
}
VariableInfo Manager::checkIfSymbolExist(std::string name){
    return SymbolTable[name];
}

//========================= Variable ================================

void Manager::registerVariable(std::string name,Type type, std::string value){
    if(SymbolTable.count(name) != 0 && type==Type::String) return;
    if(SymbolTable.count(name) == 0){
        SymbolTable.insert(std::pair<std::string, VariableInfo>(name,VariableInfo(name,type,value)));
    }
    else 
        yyerror("ManagerError: Zmienna już istnieje!");
}
void Manager::registerVariable(std::string name,Type type, std::string value, std::vector<std::string> dim, std::vector<int> size, int arrSize){
    if(SymbolTable.count(name) != 0 && type==Type::String) return;
    if(SymbolTable.count(name) == 0){
        SymbolTable.insert(std::pair<std::string, VariableInfo>(name,VariableInfo(name,type,value,dim,size,arrSize)));
    }
    else 
        yyerror("ManagerError: Zmienna już istnieje!");
}
void Manager::registerVariable(std::string name, Type type){
    registerVariable(name,type,"0");
}
void Manager::registerVariable(VariableInfo variable){
    registerVariable(variable._name,variable._type,variable._value, variable._dim,variable._size,variable.arraySize);
}
void Manager::registerVariable(){
    auto var = getItemFromVariableStack();
    var.calculateSizes();
    if(var.arraySize > 0)
        registerVariable(var._name,var._type,"0:"+util.toString(var.arraySize),var._dim,var._size,var.arraySize);
    else
        registerVariable(var._name,var._type,"0");
}
void Manager::registerString(Type type, std::string value){
    std::string stringId = "string" + util.toString(StringIterator++);
    registerVariable(stringId, type, value);
    pushVariableOnStack(stringId, type, value);
}
VariableInfo Manager::getItemFromVariableStack(){
    if(VariableStack.size()== 0)
        yyerror("ManagerError: Stos operacyjny jest pusty!");
    VariableInfo v = VariableStack.top();
    VariableStack.pop();
    return v;
}
void Manager::pushVariableOnStack(VariableInfo variable){
    VariableStack.push(variable);
}
void Manager::pushVariableOnStack(std::string name, Type type){
    pushVariableOnStack(name,type,"0");
}
void Manager::pushVariableOnStack(std::string name, Type type, std::string value){
    pushVariableOnStack(VariableInfo(name,type,value));
}
void Manager::pushExistingVariable(std::string name){
    if(SymbolTable.count(name) != 0){
        pushVariableOnStack(SymbolTable[name]);
    }else
        yyerror("ManagerError: Zmienna musi być pierw stworzona!");
}

//======================== Expression ===============================

std::string Manager::registerExpresionValue(Type type){
    std::string resultId = "result" + util.toString(ExpressionIterator++);
    registerVariable(resultId, type, "0");
    pushVariableOnStack(resultId, type, "0");
    return resultId;
}


//========================== Label ==================================

std::string Manager::registerLabel(){
    std::string id = "LABEL"+util.toString(LabelIterator++);
    LabelStack.push(id);
    return id;
}
bool Manager::isLabelStackEmpty(){
    return LabelStack.empty();
}
int Manager::getLabelStackSize(){
    return LabelStack.size();
}
void Manager::pushLabelOnStack(std::string label){
    LabelStack.push(label);
}
std::string Manager::getItemFromLabelStack(){
    if(LabelStack.size() == 0)
        yyerror("ManagerError: Stos etykiet jest pusty!");
    std::string id = LabelStack.top();
    LabelStack.pop();
    return id;
}

//========================= Function ================================

void Manager::registerFunction(std::string functionName, Type type){
    if(isFunctionDeclared(functionName))
        yyerror("ManagerError: Funkcja już istnieje!");
    
    FunctionTable.insert(std::pair<std::string, Type>(functionName, type));
}
bool Manager::isFunctionDeclared(std::string name){
    return FunctionTable.count(name) != 0;
}
std::map<std::string, Type> Manager::getFunctionTable(){
    return FunctionTable;
}