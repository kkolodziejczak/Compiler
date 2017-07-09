#include "Generator.hpp"

//=========================== Utils =================================

std::vector<std::string> Generator::generateCodeForVariables(){
  std::vector<std::string> output;
  output.push_back(".data\n");
  output.push_back(space + "newLine:");
  output.push_back(space + ".asciiz");
  output.push_back(space + "\"\\n\"\n");

  for(auto p: manager->getSymbolTable()){
    output.push_back(space + p.first + ":");
    output.push_back(space + util.TypeToString(p.second._type));
    output.push_back(space + p.second._value + " \n");
  }
  return output;
}

void Generator::writeCode(std::string code, bool applaySpace){
  if(applaySpace)
    ProgramCode.push_back(space + code);
  else
    ProgramCode.push_back(code);
}

std::vector<std::string> Generator::getProgramCode(){
  if(manager->isFunctionDeclared("main"))
    return ProgramCode;
  else
    yyerror("GeneratorError: Funkcja \"main\" musi zostać zadeklarowana!");
}

//============================ I/O ==================================

void Generator::generateCodeForPrint(bool newLine){

  VariableInfo variable = manager->getItemFromVariableStack();
  auto symbol =  manager->getSymbolTable();
  auto c = symbol[variable._name];

    switch(c._type){
      case Integer:
        writeCode("li\t\t$v0, 1\n");
      break;
      case Float:
        writeCode("li\t\t$v0, 2\n");
      break;
      case String:
        writeCode("li\t\t$v0, 4\n");
      break;
    }

    switch(c._type){
      case String:
        writeCode("la \t\t$a0, "+variable._name+"\n");
      break;
      case Integer:
        if(isArray(variable._name)){
          writeCode("la\t\t$t4, " + variable._name + "\n");
          getArrayPosition(variable);
          // writeCode("li\t\t$t5, " + getArrayPosition(variable) + "\n");
          writeCode("add\t\t$t4, $t4, $t5\n");
          writeCode("lw\t\t$t0, ($t4)\n");
          writeCode("add\t\t$a0, $t0, $zero \n");
        }
        else{
          writeCode("lw \t\t$a0, "+variable._name+"\n");
          // writeCode("lw \t\t$t0, "+variable._name+"\n");
          // writeCode("add\t\t$a0, $t0, $zero \n");
        }
      break;
      case Float:
        if(isArray(variable._name)){
          writeCode("la\t\t$t4, " + variable._name + "\n");
          getArrayPosition(variable);
          // writeCode("li\t\t$t5, " + getArrayPosition(variable) + "\n");
          writeCode("add\t\t$t4, $t4, $t5\n");
          writeCode("l.s\t\t$f12, ($t4)\n");
        }
        else
          writeCode("l.s \t$f12, "+variable._name+"\n");
      break;
      
    }
  

  writeCode("syscall\n\n");

  if(newLine){
    writeCode("li\t\t$v0, 4\n");
    writeCode("la \t\t$a0, newLine\n");
    writeCode("syscall\n\n");
  }
}

void Generator::generateCodeForInput(std::string name){
  
  VariableInfo var = manager->getSymbolTable()[name];
  if(manager->getSymbolTable().count(name) == 0)
    yyerror("GeneratorError: Zmienna musi być zadeklarowana aby wczytać do niej wartość!");
  

  switch(var._type){
    case Integer:
      writeCode("li\t\t$v0, 5\n");
      break;
    case Float:
      writeCode("li\t\t$v0, 6\n");
      break;
  } 
  writeCode("syscall\n");
  
  switch(var._type){
    case Integer:
      if(isArray(var._name)){
          writeCode("la\t\t$t4, " + var._name + "\n");
          getArrayPosition(var);
          writeCode("add\t\t$t4, $t4, $t5\n");
          writeCode("sw\t\t$v0, ($t4)\n");
      }else
        writeCode("sw\t\t$v0, "+var._name+"\n\n");
      break;
    case Float:
      if(isArray(var._name)){
          writeCode("la\t\t$t4, " + var._name + "\n");
          getArrayPosition(var);
          writeCode("add\t\t$t4, $t4, $t5\n");
          writeCode("s.s\t\t$f0, ($t4)\n");
      }else
        writeCode("s.s\t\t$f0, "+var._name+"\n\n");
      break;
  }
  util.DebugPrint("Test");
  
}

//======================== Expression ===============================

void Generator::generateCodeForExpression(char ExprSymbol){
    VariableInfo var2 = manager->getItemFromVariableStack();
    VariableInfo var1 = manager->getItemFromVariableStack();

    util.DebugPrint("Var1");
    var1.print();
    switch(var1._type){
      case Integer:
        if(isdigit(var1._name[0]))
            writeCode("li\t\t$t0, "+var1._name+"\n");
        else
          if(isArray(var1._name)){
            writeCode("la\t\t$t4, " + var1._name + "\n");
            getArrayPosition(var1);
            // writeCode("li\t\t$t5, " + getArrayPosition(var1) + "\n");
            writeCode("add\t\t$t4, $t4, $t5\n");
            writeCode("lw\t\t$t0, ($t4)\n");
          }
          else
            if(ExprSymbol != '=')
              writeCode("lw\t\t$t0, "+var1._name+"\n"); 
        break;
      case Float:
        if(isdigit(var1._name[0])){
          writeCode("li\t\t$t0, "+util.stringToFloatAsInteger(var1._name)+"\n");
          writeCode("mtc1\t\t$t0, $f0\n");
        }
        else
          if(isArray(var1._name)){
            writeCode("la\t\t$t4, " + var1._name + "\n");
            writeCode("li\t\t$t5, " + getArrayPosition(var1) + "\n");
            writeCode("add\t\t$t4, $t4, $t5\n");
            writeCode("lw\t\t$t0, ($t4)\n");
            writeCode("mtc1\t\t$t0, $f0\n");
          }
          else
            if(ExprSymbol != '=')
              writeCode("l.s\t\t$f0, "+var1._name+"\n");
        break;
      case Function:
      {
        auto functionTable = manager->getFunctionTable();
        switch(functionTable[var1._name]){
          case Integer:
            writeCode("add\t\t$t0, $a0, $zero\n");
          break;
          case Float:
            writeCode("add\t\t$t0, $a0, $zero\n");
            writeCode("mtc1\t\t$t0, $f0\n");
          break;
        }
        break;
      }
      default:
        yyerror("Nie właściwy typ zmiennej!");
      break;
    }

    util.DebugPrint("Var2");
    var2.print();
    switch(var2._type){
      case Integer:
        if(isdigit(var2._name[0]))
          writeCode("li\t\t$t1, "+var2._name+"\n");
        else
          if(isArray(var2._name)){
            writeCode("la\t\t$t4, " + var2._name + "\n");
            // writeCode("li\t\t$t5, " + getArrayPosition(var2) + "\n");
            getArrayPosition(var2);
            writeCode("add\t\t$t4, $t4, $t5\n");
            writeCode("lw\t\t$t1, ($t4)\n");
          }
          else
              writeCode("lw\t\t$t1, "+var2._name+"\n");
        break;
      case Float:
        if(isdigit(var2._name[0])){
          writeCode("li\t\t$t1, "+util.stringToFloatAsInteger(var2._name)+"\n");
          writeCode("mtc1\t\t$t1, $f1\n");
        }
        else
          if(isArray(var2._name)){
        //TODO Możliwy problem z wyrażeniami jako odwołaniem do tablicy!
            writeCode("la\t\t$t4, " + var2._name + "\n");
            getArrayPosition(var2);
            // writeCode("li\t\t$t5, " + getArrayPosition(var2) + "\n");
            writeCode("add\t\t$t4, $t4, $t5\n");
            writeCode("lw\t\t$t1, ($t4)\n");
            writeCode("mtc1\t\t$t1, $f1\n");
          }
          else
              writeCode("l.s\t\t$f1, "+var2._name+"\n");
        break;
      case Function:
      {
        auto functionTable = manager->getFunctionTable();
        switch(functionTable[var1._name]){
          case Integer:
            writeCode("add\t\t$t1, $a0, $zero\n");
          break;
          case Float:
            writeCode("add\t\t$t1, $a0, $zero\n");
            writeCode("mtc1\t\t$t1, $f1\n");
          break;
        }
        break;
      }
      default:
        yyerror("Nie właściwy typ zmiennej!");
      break;
    }

    switch(ExprSymbol){
      case '=':
      if(var1._type != var2._type)
        yyerror("GeneratorError: Typy muszą być takie same!");
        switch(var2._type){
          case Integer:
            writeCode("sw\t\t$t1, "+ var1._name+"\n\n");
            break;
          case Float:
            writeCode("s.s\t\t$f1, "+ var1._name+"\n\n");
            break;
        }
      break;
      case '+':
        generateCodeForExpressionSymbol("add",var1,var2);
      break;
      case '-':
        generateCodeForExpressionSymbol("sub",var1,var2);
      break;
      case '*':
        generateCodeForExpressionSymbol("mul",var1,var2);
      break;
      case '/':
        generateCodeForExpressionSymbol("div",var1,var2);
      break;
      default: yyerror("GeneratorError[Stos->ASM]: Błędny operator\n");
      break;
    }
    
    if(ExprSymbol != '='){
      Type type = Type::Float;
        if(var1._type == Type::Integer || var2._type == Type::Integer)
          type = Type::Integer;
      
      std::string id = manager->registerExpresionValue(type);
        switch(var1._type){
          case Integer:
              writeCode("sw\t\t$t0, "+id+"\n\n");
            break;
          case Float:
            switch(var2._type){
              case Integer:
                writeCode("sw\t\t$t0, "+id+"\n\n");
              break;
              case Float:
                writeCode("s.s\t\t$f0, "+id+"\n\n");
              break;
            }
            break;
        }
    }
}

void Generator::generateCodeForExpressionSymbol(
  std::string command, VariableInfo var1, VariableInfo var2){
  switch(var1._type){
    case Integer:
      switch(var2._type){
        case Integer:
          writeCode(command+"\t\t$t0, $t0, $t1\n");
        break;
        case Float:
          writeCode("cvt.w.s\t\t$f0, $f1\n");
          writeCode("mfc1\t\t$t1, $f0\n");
          writeCode(command+"\t\t$t0, $t0, $t1\n");
        break;
      }
      break;
    case Float:
      switch(var2._type){
        case Integer:
          writeCode("cvt.w.s\t\t$f0, $f0\n");
          writeCode("mfc1\t\t$t0, $f0\n");
          writeCode(command+"\t\t$t0, $t0, $t1\n");
        break;
        case Float:
          writeCode(command+".s\t\t$f0, $f0, $f1\n");
        break;
      }
      break;
  }
}

void Generator::generateCodeForModulo(){
    writeCode("mfhi\t\t$t7\n\n");
    std::string id = manager->registerExpresionValue(Type::Integer);
    writeCode("sw\t\t$t7, "+id+"\n\n");
}

//========================== Arrays =================================

void Generator::generateCodeForArrayAssignment(){
  VariableInfo varWhat = manager->getItemFromVariableStack();
  VariableInfo varWhere = manager->getItemFromVariableStack();
  
  // varWhat.print();
  // varWhere.print();

  if(varWhat._type == Type::String)
    yyerror("GeneratorError: Typ String jest nie dozwolony w tym miejscu!");

  if(varWhat._type == Type::Integer)
    if(isdigit(varWhat._name[0]))
      writeCode("li\t\t$t0, " + varWhat._name + "\n");
    else
      writeCode("lw\t\t$t0, " + varWhat._name + "\n");
  else{
    if(isdigit(varWhat._name[0])){
      writeCode("li\t\t$t0, "+util.stringToFloatAsInteger(varWhat._name)+"\n");
      writeCode("mtc1\t\t$t0, $f0\n");
    }else
      writeCode("l.s\t\t$f0, "+ varWhat._name +"\n");
  }

  writeCode("la\t\t$t4, " + varWhere._name + "\n");
  
  // if(isdigit(varWhat._name[0]))
    getArrayPosition(varWhere);
  // else
    // writeCode("lw\t\t$t5, " + varWhere._name + "\n");
  
  writeCode("add\t\t$t4, $t4, $t5\n");
  if(varWhat._type == Type::Integer)
    writeCode("sw\t\t$t0, ($t4)\n\n");
  else{
    writeCode("s.s\t\t$f0, ($t4)\n\n");
  }
}

std::string Generator::getArrayPosition(VariableInfo var){
    int size = var._size.size();
    std::vector<std::string> array(&var._dim[size],&var._dim[var._dim.size()]);

    int arrayShiftVar = 0;
    bool declaration = true;

    for(int i =0; i < var._size.size(); i++)
      if(!isdigit(array[i][0]))
        declaration= false;

    if(declaration){
      for(int i =0; i < var._size.size(); i++)
        arrayShiftVar += var._size[i]*std::atoi(array[i].c_str());

      writeCode("li\t\t$t5, " + util.toString(arrayShiftVar) + "\n");

    }else{
      writeCode("li\t\t$t5, 0\n");
      for(int i =0; i < var._size.size(); i++){

        writeCode("li\t\t$t2, " + util.toString(var._size[i]) + "\n");
        
        if(isdigit(array[i][0]))
          writeCode("li\t\t$t3, " + array[i] + "\n");
        else
          writeCode("lw\t\t$t3, " + array[i] + "\n");

        writeCode("mul\t\t$t2, $t2, $t3\n");
        writeCode("add\t\t$t5, $t5, $t2\n");
      }
    }
    return util.toString(arrayShiftVar);
}

bool Generator::isArray(std::string variableName){
  auto symbolTable = manager->getSymbolTable();
  auto functionTable = manager->getFunctionTable();

  if(symbolTable.count(variableName) != 0 || functionTable.count(variableName) != 0){
    if(symbolTable[variableName]._dim.size() > 0)
      return true;
    else
      return false;
  }else
    if(isdigit(variableName[0]))
      return false;
    yyerror("GeneratorError: Zmienna musi być pierw stworzona!");
}

//============================ IF ===================================

void Generator::generateCodeForIfStatement(LogicExpr expr, bool whileStatement){

  if(!whileStatement)
    auto OutLabel = manager->registerLabel();

  auto ElseLabel = manager->registerLabel();
  auto TrueLabel = manager->registerLabel();


  VariableInfo var1 = manager->getItemFromVariableStack();
  switch(var1._type){
    case Integer:
      if(isdigit(var1._name[0]))
        writeCode("li\t\t$t2, " + var1._name + "\n");
      else
        writeCode("lw\t\t$t2, " + var1._name + "\n");
      break;
    case Float:
      if(isdigit(var1._name[0])){
        writeCode("li\t\t$t0, "+util.stringToFloatAsInteger(var1._name)+"\n");
        writeCode("mtc1\t\t$t0, $f2\n");
      }
      else
        writeCode("l.s\t\t$f2, " + var1._name + "\n");
      break;
  }

  VariableInfo var2 = manager->getItemFromVariableStack();
  switch(var2._type){
    case Integer:
      if(isdigit(var2._name[0]))
        writeCode("li\t\t$t3, " + var2._name + "\n");
      else
        writeCode("lw\t\t$t3, " + var2._name + "\n");
      break;
    case Float:
      if(isdigit(var2._name[0])){
        writeCode("li\t\t$t0, "+util.stringToFloatAsInteger(var2._name)+"\n");
        writeCode("mtc1\t\t$t0, $f3\n");
      }
      else
        writeCode("l.s\t\t$f3, " + var2._name + "\n");
      break;
  }


  switch(expr){
    case Equal:
      if(var1._type == Type::Integer)
        writeCode("beq\t\t$t2, $t3 "+TrueLabel+ "\n");
      else
        writeCode("c.eq.s\t\t$f2, $f3\n");
      break;
    case NotEqual:
      if(var1._type == Type::Integer)
        writeCode("bne\t\t$t2, $t3 "+TrueLabel+ "\n");
      else
        yyerror("GeneratorError: nie można używać != dla licz zmiennoprzecinkowych!");
      break;
    case Greater:
      if(var1._type == Type::Integer)
        writeCode("bgt\t\t$t2, $t3 "+TrueLabel+ "\n");
      else
        yyerror("GeneratorError: nie można używać > dla licz zmiennoprzecinkowych!");
      break;
    case GreaterEqual:
      if(var1._type == Type::Integer)
        writeCode("bge\t\t$t2, $t3 "+TrueLabel+ "\n");
      else
        yyerror("GeneratorError: nie można używać >= dla licz zmiennoprzecinkowych!");
      break;
    case Less:
      if(var1._type == Type::Integer)
        writeCode("blt\t\t$t2, $t3 "+TrueLabel+ "\n");
      else
        writeCode("c.lt.s\t\t$f2, $f3\n");
      break;
    case LessEqual:
      if(var1._type == Type::Integer)
        writeCode("ble\t\t$t2, $t3 "+TrueLabel+ "\n");
      else
        writeCode("c.le.s\t\t$f2, $f3\n");
      break;
  }
  if(var1._type == Type::Float)
    writeCode("bc1t "+TrueLabel+ "\n");

  writeCode("b " + ElseLabel + "\n\n");
  generateCodeForJump();
}

std::string Generator::generateCodeForJump(){
  std::string label = manager->getItemFromLabelStack();
  writeCode(label + ":\n",false);
  return label;
}

void Generator::generateJumpToLabel(){
  auto label = manager->getItemFromLabelStack();
  auto label2 = manager->getItemFromLabelStack();
  writeCode("b " + label2 + "\n\n");
  manager->pushLabelOnStack(label2);
  manager->pushLabelOnStack(label);
}

void Generator::generateCodeForWhileEnd(){
  auto label = manager->getItemFromLabelStack();
  auto label2 = manager->getItemFromLabelStack();
  writeCode("b " + label2 + "\n\n");
  manager->pushLabelOnStack(label);
  generateCodeForJump();
}

//========================= Function ================================

void Generator::generateCodeForFunctionJump(std::string functionName){
  if(!manager->isFunctionDeclared(functionName))
    yyerror("GeneratorError: Funkcja nie została zadeklarowana!");

    writeCode("jal "+functionName+"\n\n");
    
    auto func = manager->getFunctionTable();
    auto funcType = func[functionName];
    std::string id = manager->registerExpresionValue(funcType);

    switch(funcType){
      case Integer:
          writeCode("sw\t\t$a0, "+id+"\n\n");
        break;
      case Float:
          writeCode("add\t\t$t0, $a0, $zero\n");
          writeCode("mtc1\t\t$t0, $f3\n");
          writeCode("s.s\t\t$f3, "+id+"\n\n");
        break;
    }
}

void Generator::generateCodeForFunction(std::string functionName){
  if(manager->isFunctionDeclared(functionName))
    writeCode(functionName+":\n",false);
}

void Generator::generateCodeForFunctionJumpOut(std::string functionName){
   if(manager->isFunctionDeclared(functionName))
    if(functionName == "main")
      writeCode("\n");
    else
      writeCode("jr $ra\n\n");
}

void Generator::generateCodeForReturnValue(){
  auto var = manager->getItemFromVariableStack();
  //INTEGER
  // li		$t0, 10
	// add		$a0, $t0, $zero
  //FLOAT
  // mfc1		$t1, $f0
  // add		$a0, $t1, $zero
	switch (var._type){
    case Integer:
        if(isdigit(var._name[0]))
          writeCode("li\t\t$t0, "+var._name+"\n");
        else
          writeCode("lw\t\t$t0, "+var._name+"\n");
        writeCode("add\t\t$a0, $t0, $zero\n\n");
      break;
    case Float:
        if(isdigit(var._name[0])){
          writeCode("li\t\t$t0, "+util.stringToFloatAsInteger(var._name)+"\n");
          writeCode("mtc1\t\t$t0, $f0\n");
        }
        else
          writeCode("l.s\t\t$f0, "+var._name+"\n");
        writeCode("mfc1\t\t$t0, $f0\n");
        writeCode("add\t\t$a0, $t0, $zero\n\n");
      break;
    default:
      break;
  }
}
