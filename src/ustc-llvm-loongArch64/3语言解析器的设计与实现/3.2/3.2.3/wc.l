%option noyywrap
%{
#include <string.h>
int chars = 0;
int words = 0;
%}

%%
[a-zA-Z]+  { chars += strlen(yytext); words++; }
.  {}
%%

int main()
{
    yylex();
    printf("look, I find %d words of %d chars\n", words, chars);
    return 0;
}