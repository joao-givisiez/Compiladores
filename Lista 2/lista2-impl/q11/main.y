/* Q11 - Declaracoes de variaveis em Pascal (simplificado) */
%{
#include <stdio.h>
#include <stdlib.h>
int yylex(void);
void yyerror(const char *s);
extern int yylineno;
%}

/* Terminais: palavras reservadas, identificador e pontuacao */
%token VAR INTEGER CHAR REAL ID VIRGULA DOISPONTOS PONTOVIRGULA

%start D

%%

/* D -> var V */
D : VAR V
  ;

/* V -> V L : T ; | L : T ; */
V : V L DOISPONTOS T PONTOVIRGULA
  | L DOISPONTOS T PONTOVIRGULA
  ;

/* L -> L , id | id */
L : L VIRGULA ID
  | ID
  ;

/* T -> integer | char | real */
T : INTEGER
  | CHAR
  | REAL
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
