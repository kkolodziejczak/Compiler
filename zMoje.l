%{
#include "def.tab.hh"
#include <stdlib.h>
#include <string.h>
extern int yylineno;
int yyerror(const char *,...);
%}
%%
"int"       {return INT;}
"float"     {return FLOAT;}
"string"    {return STRING;}
"for"       {return FOR;}
"while"     {return WHILE;}
"do"        {return DO;}
"print"     {return PRINT;}
"println"   {return PRINTLN;}
"function"   {return FUNCTION;}
"return"   {return RETURN;}
"in"        {return IN;}
"if"        {return IF;}
"else"      {return ELSE;}
\+\+        {return INC;}
\-\-        {return DEC;}
\%          {return MOD;}
\.          {return '.';}
\,          {return ',';}
\[          {return '[';}
\]          {return ']';}
\{          {return '{';}
\}          {return '}';}
\(          {return '(';}
\)          {return ')';}
\+          {return '+';}
\-          {return '-';}
\*          {return '*';}
\/          {return '/';}
\+\=        {return ADEQ;}
\-\=        {return SUEQ;}
\*\=        {return MUEQ;}
\/\=        {return DIEQ;}
\!          {return NOT;}
\=\=        {return EQ;}
\&\&        {return AND;}
\|\|        {return OR;}
\^          {return XOR;}
\>          {return GRE;}
\<          {return LES;}
\!\=        {return DIF;}
\=          {return '=';}
\<\=        {return LEQ;}
\>\=        {return GEQ;}
\/\/        {return COM;}
\>\>        {return SHR;}
\<\<        {return SHL;}
\;          {return ';';}
[A-Z_a-z][A-Za-z_0-9]*  {yylval.text = strdup(yytext);return VAR;}
([1-9][0-9]*|0) {yylval.ival = atoi(yytext);return IVAL;}
[\"].*?[\"]  {yylval.strtext = strdup(yytext);return SVAL;}
[0-9]+[\.][0-9]+ {yylval.fval = atof(yytext);return FVAL;}
\ |\t       {;}

\n          {yylineno++; }
.     {yyerror("[LEKS]: Unknown leksem.");}
%%
int yyerror(const char *msg,...)
{
  printf("[line %d]%s\n", yylineno, msg);
  exit(1);
}