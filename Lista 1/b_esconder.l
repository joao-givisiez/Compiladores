%option noyywrap yylineno

%{
#include <stdio.h>
%}

%x OCULTO

%%

"<ESCONDER>"            { printf("%s", yytext); BEGIN(OCULTO); }

<OCULTO>"</ESCONDER>"   { printf("%s", yytext); BEGIN(INITIAL); }
<OCULTO>\n              { printf("\n"); }
<OCULTO>.               { printf("x"); }

.|\n                    { printf("%s", yytext); }

%%
int main(int argc, char *argv[]) {
    if (argc > 1) yyin = fopen(argv[1], "r");
    yylex();
    if (argc > 1) fclose(yyin);
    return 0;
}
