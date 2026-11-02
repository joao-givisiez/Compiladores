%{
#include "parser.tab.h"
%}

%option noyywrap yylineno

%%

[ \t\r\n]+               { /* ignora espacos, tabs e quebras de linha */ }

 /* palavras reservadas: vem antes de id para terem prioridade */
"var"                    { return VAR; }
"integer"                { return INTEGER; }
"char"                   { return CHAR; }
"real"                   { return REAL; }
"array"                  { return ARRAY; }
"of"                     { return OF; }

[A-Za-z_][A-Za-z0-9_]*   { return ID; }

 /* apenas inteiros: "1..10" vira NUM, PONTOPONTO, NUM */
[0-9]+                   { return NUM; }

".."                     { return PONTOPONTO; }
","                      { return VIRGULA; }
":"                      { return DOISPONTOS; }
";"                      { return PONTOVIRGULA; }
"["                      { return ABRECOL; }
"]"                      { return FECHACOL; }

.                        { /* qualquer outro caractere e invalido */
                           printf("Erro lexico na linha %d: '%s'\n", yylineno, yytext);
                           exit(1); }

<<EOF>>                  { return 0; }

%%
