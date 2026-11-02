%{
#include "parser.tab.h"
%}

%option noyywrap yylineno

%%

[ \t\r\n]+   { /* ignora espacos, tabs e quebras de linha */ }

"a"          { return A; }
"("          { return ABRE; }
")"          { return FECHA; }
","          { return VIRGULA; }

.            { /* qualquer outro caractere e invalido */
               printf("Erro lexico na linha %d: '%s'\n", yylineno, yytext);
               exit(1); }

<<EOF>>      { return 0; }

%%
