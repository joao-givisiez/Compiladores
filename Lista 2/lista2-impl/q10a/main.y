/* Q10 - Gramatica A:  S -> ( L ) | a     L -> L , S | S */
%{
#include <stdio.h>
#include <stdlib.h>
int yylex(void);
void yyerror(const char *s);
extern int yylineno;
%}

/* Terminais: a letra 'a', parenteses e virgula */
%token A ABRE FECHA VIRGULA

%start S

%%

/* S -> ( L ) | a */
S : ABRE L FECHA
  | A
  ;

/* L -> L , S | S  (recursao a esquerda, aceita pelo LALR(1)) */
L : L VIRGULA S
  | S
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
