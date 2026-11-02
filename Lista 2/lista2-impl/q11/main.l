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

[A-Za-z_][A-Za-z0-9_]*   { return ID; }

","                      { return VIRGULA; }
":"                      { return DOISPONTOS; }
";"                      { return PONTOVIRGULA; }

.                        { /* qualquer outro caractere e invalido */
                           printf("Erro lexico na linha %d: '%s'\n", yylineno, yytext);
                           exit(1); }

<<EOF>>                  { return 0; }

%%
