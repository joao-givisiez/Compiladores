%option noyywrap yylineno

%{
#include <stdio.h>
%}

letra       [A-Za-z]
digito      [0-9]
espaco      [ \t\n\r]

%%

{espaco}+               { }

#[^#]*#                 { printf("10 %s\n", yytext); }

\"[^\"]*\"              { printf("07 %s\n", yytext); }

begin|end|for|do|if|then|else|elseif|endif|while|repeat|until  { printf("03 %s\n", yytext); }

integer|real|float|double|char|string|boolean  { printf("04 %s\n", yytext); }

SQR|SQRT|SIN|COS|TAN    { printf("02 %s\n", yytext); }

_{letra}({letra}|{digito})*   { printf("05 %s\n", yytext); }

\${letra}({letra}|{digito})*  { printf("06 %s\n", yytext); }

{letra}+                { printf("09 %s\n", yytext); }

[+-]?{digito}+          { printf("08 %s\n", yytext); }

"<="|">="|"=<"|"=>"|"<>"|"<"|">"|"="  { printf("01 %s\n", yytext); }

[.,:;]                  { printf("00 %s\n", yytext); }

[+\-*/]                 { printf("02 %s\n", yytext); }

.                       { printf("99 %s\n", yytext); }

%%
int main(int argc, char *argv[]) {
    if (argc > 1) yyin = fopen(argv[1], "r");
    yylex();
    if (argc > 1) fclose(yyin);
    return 0;
}
