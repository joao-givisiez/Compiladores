/* Q12 - Q11 acrescida do tipo array do Pascal */
%{
#include <stdio.h>
#include <stdlib.h>
int yylex(void);
void yyerror(const char *s);
extern int yylineno;
%}

/* Terminais: palavras reservadas, identificador, numero inteiro e pontuacao */
%token VAR INTEGER CHAR REAL ARRAY OF ID NUM
%token VIRGULA DOISPONTOS PONTOVIRGULA ABRECOL FECHACOL PONTOPONTO

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

/* T -> integer | char | real | array [ NUM .. NUM ] of T */
T : INTEGER
  | CHAR
  | REAL
  | ARRAY ABRECOL NUM PONTOPONTO NUM FECHACOL OF T
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
