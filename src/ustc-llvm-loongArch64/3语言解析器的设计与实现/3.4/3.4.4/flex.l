%option noyywrap
%{
#include "bison.tab.h"  // 引入Bison生成的头文件
%}

%%
"int"			{ return INT; }
"main"		{ return MAIN; }
"("			{ return LPAREN; }
")"			{ return RPAREN; }
"{"			{ return LBRACE; }
"}"			{ return RBRACE; }
";"			{ return SEMICOLON; }
"return"		{ return RETURN; }
\n 			{ /* 忽略 */ }
. 			{ /* 忽略 */ }
%%