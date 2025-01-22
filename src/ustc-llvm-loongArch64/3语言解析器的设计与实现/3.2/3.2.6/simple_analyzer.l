%option noyywrap
%{
/* %{ ... %}是纯C语法，
声明部分会被Flex直接复制到生成的C文件的开头部分。
这里声明了一些全局变量来统计行数、匹配文本开头和结尾。*/
#include <stdio.h>
#include <stdlib.h>

int lines;
int pos_start;
int pos_end;

#define ADD 1
#define MUL 2
#define DIV 3
#define SUB 4
#define ASSIN 5
#define SEMICOLON 6
#define COMMA 7
#define LPARENTHESE 8
#define RPARENTHESE 9
#define INT 10
#define IDENTIFIER 11
#define INTEGER 12
#define ERROR 0
%}

%%

\+ 	{pos_start = pos_end; pos_end += 1;   return ADD;}
\-	{pos_start = pos_end; pos_end += 1;   return SUB;}
\*	{pos_start = pos_end; pos_end += 1;   return MUL;}
\/	{pos_start = pos_end; pos_end += 1;   return DIV;}
=	{pos_start = pos_end; pos_end += 1;   return ASSIN;}
;	{pos_start = pos_end; pos_end += 1;   return SEMICOLON;}
,	{pos_start = pos_end; pos_end += 1;   return COMMA;}
\(	{pos_start = pos_end; pos_end += 1;   return LPARENTHESE;}
\)	{pos_start = pos_end; pos_end += 1;   return RPARENTHESE;}
int	{pos_start = pos_end; pos_end += 3;   return INT;}
[a-zA-Z]+	{pos_start = pos_end; pos_end += strlen(yytext);   return IDENTIFIER;}
[0-9]+	{pos_start = pos_end; pos_end += strlen(yytext);   return INTEGER;}
\n      {lines++; pos_start = 1; pos_end = 1;}
\r\n    {lines++; pos_start = 1; pos_end = 1;}
[ \t] 	{pos_start = pos_end; pos_end += 1;}

. { pos_start = pos_end; pos_end++; return ERROR; }  /* 其他所有字符 */

%%
 /*user code 调用词法分析的入口 这里调用词法分析的入口在.y中*/