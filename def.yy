%{
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <map>

#include "Utils.hpp"
#include "VariableInfo.hpp"
#include "Manager.hpp"
#include "Generator.hpp"

#define INFILE_ERROR 1
#define OUTFILE_ERROR 2
#define DEBUG 1

#if DEBUG
#define LOG(x) std::cout << x << std::endl;
#else
#define LOG(x)
#endif

extern FILE *yyin,*yyout;
extern "C" int yylex();
extern "C" int yyerror(const char*msg,...);

Utils util;
Manager manager;
std::ofstream outfile;
Generator generator(&manager);
bool WhileFlag = false;

void notInFunctionError(){
  yyerror("Wszystkie operacje musą odbywać się w funkcjach!");
}

%}
%union
{
char *text;
char *strtext;
int ival;
float fval;
}
%token EQ DIF NOT LEQ GEQ GRE LES AND OR XOR MOD
%token INT FLOAT STRING
%token FOR WHILE DO IF ELSE
%token PRINT PRINTLN IN FUNCTION RETURN
%token ADEQ SUEQ MUEQ DIEQ
%token INC DEC SHR SHL
%token COM

%token <text> VAR
%token <strtext> SVAL
%token <ival> IVAL
%token <fval> FVAL
%%

programLines
  :
  | programLines programLine {;}
;
programLine
  : deklaracjaFunkcji
  | tablice ';' {notInFunctionError();}
  | tablicePrzypisanie ';'  {notInFunctionError();}
  | wywolanieFunkcji ';' {notInFunctionError();} 
  | deklaracja ';' {notInFunctionError();}
  | wyswietlenie ';'  {notInFunctionError();}
  | wczytywanie ';' {notInFunctionError();}
  | petlaWhile {notInFunctionError();}
  | returnValue ';' {notInFunctionError();}
  | if_expr {notInFunctionError();}
;
lines
  :
  | lines line {;}
;
line
  : tablice ';'
  | tablicePrzypisanie ';'
  | wywolanieFunkcji ';'
  | deklaracja ';'
  | wyswietlenie ';'
  | wczytywanie ';'
  | petlaWhile
  | returnValue ';'
  | if_expr
  | VAR INC ';' {
    LOG("post Inc");
    auto value = util.toString($1);
    manager.pushExistingVariable(value);
    manager.pushExistingVariable(value);
    manager.pushVariableOnStack("1",Type::Integer);
    generator.generateCodeForExpression('+');
    generator.generateCodeForExpression('=');
  }
  | VAR DEC ';' {
    LOG("post Dec");
    auto value = util.toString($1);
    manager.pushExistingVariable(value);
    manager.pushExistingVariable(value);
    manager.pushVariableOnStack("1",Type::Integer);
    generator.generateCodeForExpression('-');
    generator.generateCodeForExpression('=');
  }
;
petlaWhile
  : whileLogic '(' logic_expr ')' cialo_metody  {
    LOG("While!");
    generator.generateCodeForWhileEnd();
    WhileFlag = false;
    LOG("SIZE: "<< manager.getLabelStackSize());
  }
;
whileLogic
  : WHILE {
    WhileFlag = true;
    manager.registerLabel();
    manager.pushLabelOnStack(generator.generateCodeForJump());
  }
;
wyswietlenie
  : PRINT '(' wyr ')'   {
    LOG("Operaacja PRINT");
    generator.generateCodeForPrint(false);
  }
  | PRINTLN '(' wyr ')'   {
    LOG("Operaacja PRINT");
    generator.generateCodeForPrint(true);
  }
;
wczytywanie
  : IN '(' VAR ')'      {
    LOG("OPERACJA: IN.");
    auto value = util.toString($3);
    generator.generateCodeForInput(value);
  }
  | IN '(' SVAL ')' {
    yyerror("Nie można wczytać łańcucha znaków!");
  }
;
deklaracja
  : typZmiennej przypisanie {
    generator.generateCodeForExpression('=');
  }
  | typZmiennej {;}
;
deklaracjaFunkcji
  : funkcja cialo_metody {
    LOG("deklaracjaFunkcji");
    auto var = manager.getItemFromVariableStack();
    generator.generateCodeForFunctionJumpOut(var._name);
  }
;
wywolanieFunkcji
  : VAR '('')' {
    auto value = util.toString($1);
    LOG("wywolanieFunkcji: " << $1);
    generator.generateCodeForFunctionJump(value);
  }
;
funkcja
  : INT VAR '('')'{
    auto value = util.toString($2);
    LOG("Nazwa Funkcji: " << $2);
    manager.pushVariableOnStack(VariableInfo(value,Type::Function));
    manager.registerFunction(value, Type::Integer);
    generator.generateCodeForFunction(value);
  }
  | FLOAT VAR '('')'{
    auto value = util.toString($2);
    LOG("Nazwa Funkcji: " << $2);
    manager.pushVariableOnStack(VariableInfo(value,Type::Function));
    manager.registerFunction(value,Type::Float);
    generator.generateCodeForFunction(value);
  }
;
returnValue
  : RETURN wyr  {
    generator.generateCodeForReturnValue();
  }

;
typZmiennej
  : INT VAR {
    LOG("Deklaracja INT " << $2);
    auto value = util.toString($2);
    manager.registerVariable(value,Type::Integer);
    manager.pushVariableOnStack(value,Type::Integer);
  }
  | FLOAT VAR {
    LOG("Deklaracja FLOAT " << $2);
    auto value = util.toString($2);
    manager.registerVariable(value,Type::Float);
    manager.pushVariableOnStack(value,Type::Float);
  }
  | VAR {
    LOG("Odwołanie do zmiennej Przy przypisaniu" << $1);
    auto value = util.toString($1);
    manager.pushExistingVariable(value);
  }
;
przypisanie
  : '=' wyr{
    LOG("Przypisanie nie wyr!");
  }
;
tablice
  : arrayStart arraySize  {
    LOG("Deklaracja Tablicy!");
    auto var = manager.getItemFromVariableStack();
    manager.pushVariableOnStack(var);
    manager.registerVariable();
  }
;
tablicePrzypisanie
  : arrayRePush odwolanieSize przypisanie  {
    LOG("Przypisanie do tablicy wartości");
    generator.generateCodeForArrayAssignment();
  }
;
arrayStart
  : INT VAR {
    LOG("Zmienna Tablicowa Typu INT");
    auto value = util.toString($2);
    manager.pushVariableOnStack(value,Type::Integer);
  }
  | FLOAT VAR {
    LOG("Zmienna Tablicowa Typu FLOAT");
    auto value = util.toString($2);
    manager.pushVariableOnStack(value,Type::Float);
  }
;
arrayRePush
  : VAR {
    LOG("Zmienna Tablicowa RePush");
    auto value = util.toString($1);
    manager.pushExistingVariable(value);
  }
;
arraySize
  : '[' sizeExpr ']'
;
sizeExpr
  : sizeExpr ',' sizeValue
  | sizeValue
;
sizeValue
  : IVAL  {
    LOG("IVAL Dla Tablicy! " << $1);
    auto var = manager.getItemFromVariableStack();
    var.pushDim(util.toString($1));
    manager.pushVariableOnStack(var);
  }
;
odwolanieSize
  : '[' wyrExpr ']' {
    LOG("[ wyrExpr ]");
    auto var = manager.getItemFromVariableStack();
    var.print();
    manager.pushVariableOnStack(var);
  }
;
wyrExpr
  : wyrExpr ',' wyr {
    LOG(" wyrExpr , wyr");
    auto res = manager.getItemFromVariableStack();
      if(res._type == Type::Float)
        yyerror("GeneratorError: Nie można iterować po wartościach float");
    auto var = manager.getItemFromVariableStack();
    var.pushDim(res._name);
    manager.pushVariableOnStack(var);
  }
  | wyr {
    LOG(" wyrExpr => wyr");
    auto res = manager.getItemFromVariableStack();
     if(res._type == Type::Float)
        yyerror("GeneratorError: Nie można iterować po wartościach float");
    auto var = manager.getItemFromVariableStack();
    var.pushDim(res._name);
    manager.pushVariableOnStack(var);
  }
;
if_expr
      : if_begin {
        LOG("If Bez elsa");
        generator.generateCodeForJump();
      }
      | if_begin  if_else  {
        LOG("If z elsem!");
      }
;
if_begin
      : IF '(' logic_expr ')' cialo_metody { 
        LOG("Blok if");
        generator.generateJumpToLabel();
        generator.generateCodeForJump();
      }
;
if_else
      : ELSE cialo_metody {
        LOG("Blok Else")
        generator.generateCodeForJump();
        
      }
;
cialo_metody
    : '{' lines '}' {;}
    | line          {;}
;
logic_expr
      : wyr EQ wyr    {
        LOG("LOGIC: EQ");
        generator.generateCodeForIfStatement(LogicExpr::Equal,WhileFlag);
      }
      | wyr DIF wyr   {
        LOG("LOGIC: DIF");
        generator.generateCodeForIfStatement(LogicExpr::NotEqual,WhileFlag);
      }
      | wyr GRE wyr   {
        LOG("LOGIC: GRE");
        generator.generateCodeForIfStatement(LogicExpr::Less,WhileFlag);
      }
      | wyr LES wyr   {
        LOG("LOGIC: LES");
        generator.generateCodeForIfStatement(LogicExpr::Greater,WhileFlag);
      }
      | wyr LEQ wyr   {
        LOG("LOGIC: LEQ");
        generator.generateCodeForIfStatement(LogicExpr::GreaterEqual,WhileFlag);
      }
      | wyr GEQ wyr   {
        LOG("LOGIC: GEQ");
        generator.generateCodeForIfStatement(LogicExpr::LessEqual,WhileFlag);
      }
;
wyr
  : wyr '+' skladnik   {
    LOG("wyrazenie z +");
    generator.generateCodeForExpression('+');
  }
  | wyr '-' skladnik   {
    LOG("wyrazenie z -");
    generator.generateCodeForExpression('-');
  }
  | wyr MOD skladnik  {
    generator.generateCodeForExpression('/');
    generator.generateCodeForModulo();
  }
  | skladnik  {;}
;
skladnik
  : skladnik '*' czynnik  {
    LOG("wyrazenie z *");
    generator.generateCodeForExpression('*');
  }
  | skladnik '/' czynnik  {
    LOG("wyrazenie z /");
    generator.generateCodeForExpression('/');
  }
  | ujemna czynnik {
    LOG("Liczba ujemna");
    generator.generateCodeForExpression('-');
  }
  | czynnik {;}
;
ujemna
  : '-' {
    manager.pushVariableOnStack("0",Type::Integer);
  }
;
czynnik
      : VAR   {
        auto value = util.toString($1);
        LOG("Zwykła Zmienna: " << $1);
        manager.pushExistingVariable(value);
      }

      | IVAL  {
        LOG("IVAL: " << $1);
        auto value = util.toString($1);
        manager.pushVariableOnStack(value,Type::Integer);
      }
      | FVAL  {
        LOG("FVAL: "<< $1);
        auto value = util.toString($1);
        manager.pushVariableOnStack(value,Type::Float);
      }
      | SVAL  {
        LOG("SVAL: " << $1);
        auto value = util.toString($1);
        manager.registerString(Type::String,value);
      }
      | arrayRePush odwolanieSize {
        LOG("wyrazenie w [] ");
      }
      | VAR '(' ')' {
        LOG("Pushnięcie otrzymanej wartości z wywołania funkcji " << $1);
        auto value = util.toString($1);
        if(manager.isFunctionDeclared(value))
          generator.generateCodeForFunctionJump(value);
      }
      | '(' wyr ')' {;}
;
%%
int main(int argc, char *argv[]){
  if (argc>1)
  {
    yyin = fopen(argv[1], "r");
    if (yyin==NULL)
    {
      printf("Blad otworzenia pliku\n");
      return INFILE_ERROR;
    }
    if (argc>2)
    {
      outfile.open(argv[2], std::ios::out);

      if (!outfile.is_open())
      {
        printf("Blad wyjscia\n");
        return OUTFILE_ERROR;
      }
    }
  }
  yyparse();

  auto c = generator.generateCodeForVariables();
  for(auto sd : c){
    outfile << sd;
  }

  auto d = generator.getProgramCode();
  for(auto b : d){
    outfile << b;
  }

  outfile.close();
  return 0;
}