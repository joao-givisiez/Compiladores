%{
#include "parser.tab.h"
%}

%option noyywrap yylineno

%%

[ \t\r\n]+               { /* ignora espacos, tabs e quebras de linha */ }

 /* palavras reservadas: vem antes de id para terem prioridade */
"or"                     { return OR; }
"and"                    { return AND; }
"not"                    { return NOT; }
"true"                   { return TRUE; }
"false"                  { return FALSE; }

[A-Za-z_][A-Za-z0-9_]*   { return ID; }

"("                      { return ABRE; }
")"                      { return FECHA; }

.                        { /* qualquer outro caractere e invalido */
                           printf("Erro lexico na linha %d: '%s'\n", yylineno, yytext);
                           exit(1); }

<<EOF>>                  { return 0; }

%%
