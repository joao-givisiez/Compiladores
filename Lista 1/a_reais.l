%option noyywrap yylineno

%{
#include <stdio.h>
%}

digito      [0-9]
digitos     {digito}+
sinal       [+-]?
parteInt    {sinal}{digitos}
parteFrac   (\.{digitos})?
expoente    ([Ee]{sinal}{digitos})?
real        {parteInt}{parteFrac}{expoente}

%%
{real}      { printf("REAL %s\n", yytext); }
[ \t\n]+    { }
.           { printf("INVALIDO %s\n", yytext); }

%%
int main(int argc, char *argv[]) {
    if (argc > 1) yyin = fopen(argv[1], "r");
    yylex();
    if (argc > 1) fclose(yyin);
    return 0;
}
