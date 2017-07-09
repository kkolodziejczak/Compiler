#include "Utils.hpp"

// std::string Utils::toString(auto mgs){
//     ss.str("");
//     ss << mgs;
//     return ss.str();
// }

// std::string Utils::stringToFloatAsInteger(std::string value){
//     float f = std::atof(value.c_str());
//     return toString(*(int *)&f);
// }

//  std::string Utils::TypeToString(Type type){
//        switch(type){
//           case Integer:
//             return ".word";
//           break;
//           case Float:
//             return ".float";
//           break;
//           case String:
//             return ".asciiz";
//           break;
//           case Function:
//             return "Function!!!!";
//           break;
//           default:
//             yyerror("UtilsError: unknown variable type!");
//             break;
//         }
//     };

// void Utils::DebugPrint(const char * mgs){
//     printf("UTILS>DEBUG[%d]:  %s\n",iterator++,mgs);
// }