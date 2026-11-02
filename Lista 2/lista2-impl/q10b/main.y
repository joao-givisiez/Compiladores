/* Q10 - Gramatica B: expressoes booleanas */
%{
#include <stdio.h>
#include <stdlib.h>
int yylex(void);
void yyerror(const char *s);
extern int yylineno;
%}

/* Terminais: operadores, constantes, identificador e parenteses */
%token OR AND NOT TRUE FALSE ID ABRE FECHA

%start bexpr

%%

/* bexpr -> bexpr or btermo | btermo */
bexpr  : bexpr OR btermo
       | btermo
       ;

/* btermo -> btermo and bfator | bfator */
btermo : btermo AND bfator
       | bfator
       ;

/* bfator -> ( bexpr ) | not bfator | true | false | id */
bfator : ABRE bexpr FECHA
       | NOT bfator
       | TRUE
       | FALSE
       | ID
       ;

%%

/* Chamada pelo parser ao encontrar um erro sintatico */
void yyerror(const char *s) {
    (void)s;
    printf("Erro sintatico na linha %d\n", yylineno);
}

int main(void) {
    if (yyparse() == 0) printf("Sentenca aceita\n");
    return 0;
}
