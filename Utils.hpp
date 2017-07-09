#ifndef UTILS_HPP
#define UTILS_HPP

#include <sstream>
#include <cstdlib>
#include <string>
#include <stdio.h>
#include <stdlib.h>


extern "C" int yyerror(const char*msg,...);

enum LogicExpr{
  Equal,
  NotEqual,
  Greater,
  Less,
  GreaterEqual,
  LessEqual
};

enum Type{
  Integer,
  Float,
  String,
  Function
};


class Utils{
    std::stringstream ss;
    int iterator;
  public:
    std::string toString(auto mgs){
      ss.str("");
      ss << mgs;
      return ss.str();
    }
    std::string stringToFloatAsInteger(std::string value){
      float f = std::atof(value.c_str());
      return toString(*(int *)&f);
    }
    std::string TypeToString(Type type){
       switch(type){
          case Integer:
            return ".word";
          break;
          case Float:
            return ".float";
          break;
          case String:
            return ".asciiz";
          break;
          case Function:
            return "Function!!!!";
          break;
          default:
            yyerror("UtilsError: unknown variable type!");
            break;
        }
    };
    void DebugPrint(const char * mgs){
        printf("UTILS>DEBUG[%d]:  %s\n",iterator++,mgs);
    }
};




#endif