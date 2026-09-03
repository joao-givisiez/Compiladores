#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* modulo de entrada (simplificado): InicioLexema e ApontadorAdiante */
static const char *input;
static int inputLen;
static int lexemeBegin;
static int forward;

static void inputInit(const char *s) {
    input = s;
    inputLen = (int) strlen(s);
    lexemeBegin = 0;
    forward = 0;
}

/* le o proximo caractere avancando o ApontadorAdiante */
static char nextChar(void) {
    if (forward <= inputLen) return input[forward++];
    return '\0';
}

static void retract(void)        { forward--; }
static void retractN(int n)      { forward -= n; }

/* fail(): retrocede o apontador ate o InicioLexema e sinaliza rejeicao */
static int fail(void) {
    forward = lexemeBegin;
    return 0;
}

/* copia o lexema corrente para out */
static void getLexeme(char *out, int size) {
    int n = forward - lexemeBegin;
    if (n < 0) n = 0;
    if (n >= size) n = size - 1;
    memcpy(out, input + lexemeBegin, n);
    out[n] = '\0';
}

/* classes de caracteres usadas pelos diagramas */
static int isLetterLower(char c) { return c >= 'a' && c <= 'z'; }
static int isLetterC(char c)     { return isalpha((unsigned char) c); }
static int isDigitC(char c)      { return isdigit((unsigned char) c); }
static int isAlnumC(char c)      { return isalnum((unsigned char) c); }
static int isHexDigit(char c)    { return isDigitC(c) || (c >= 'A' && c <= 'F'); }

/* codigos de retorno comuns aos reconhecedores */
#define REJEITA   0
#define ACEITA    1
#define ERRO_LEX  2

/* (a) cadeias sobre {0,1} com no minimo um 0 */
static int recA(void) {
    int state = 0;
    char c;
    while (1) {
        switch (state) {
            case 0:
                c = nextChar();
                if (c == '1')      state = 0;
                else if (c == '0') state = 1;   /* achou o 0 obrigatorio */
                else return fail();
                break;
            case 1:
                c = nextChar();
                if (c == '0' || c == '1') state = 1;
                else state = 2;
                break;
            case 2:
                retract();                       /* lookahead nao pertence */
                return ACEITA;
        }
    }
}

/* (b) cadeias sobre {0,1} cujo numero de 0's e multiplo de 3 */
static int recB(void) {
    int state = 0;
    char c;
    while (1) {
        switch (state) {
            case 0:                              /* zeros = 0 (mod 3) */
                c = nextChar();
                if (c == '1')      state = 0;
                else if (c == '0') state = 1;
                else state = 3;                  /* pode aceitar aqui */
                break;
            case 1:                              /* zeros = 1 (mod 3) */
                c = nextChar();
                if (c == '1')      state = 1;
                else if (c == '0') state = 2;
                else return fail();              /* contagem nao fecha */
                break;
            case 2:                              /* zeros = 2 (mod 3) */
                c = nextChar();
                if (c == '1')      state = 2;
                else if (c == '0') state = 0;
                else return fail();
                break;
            case 3:
                retract();
                return ACEITA;
        }
    }
}

/* (c) letras minusculas contendo as cinco vogais em ordem */
static int recC(void) {
    int state = 0;
    char c;
    while (1) {
        switch (state) {
            case 0:
                c = nextChar();
                if (c == 'a')             state = 1;
                else if (isLetterLower(c)) state = 0;
                else return fail();
                break;
            case 1:
                c = nextChar();
                if (c == 'e')             state = 2;
                else if (isLetterLower(c)) state = 1;
                else return fail();
                break;
            case 2:
                c = nextChar();
                if (c == 'i')             state = 3;
                else if (isLetterLower(c)) state = 2;
                else return fail();
                break;
            case 3:
                c = nextChar();
                if (c == 'o')             state = 4;
                else if (isLetterLower(c)) state = 3;
                else return fail();
                break;
            case 4:
                c = nextChar();
                if (c == 'u')             state = 5;   /* completou aeiou */
                else if (isLetterLower(c)) state = 4;
                else return fail();
                break;
            case 5:
                c = nextChar();
                if (isLetterLower(c)) state = 5;
                else state = 6;
                break;
            case 6:
                retract();
                return ACEITA;
        }
    }
}

/* (d) letras minusculas em ordem lexicografica crescente */
static int recD(void) {
    int state = 0;
    char c, cur = 'a';
    while (1) {
        switch (state) {
            case 0:
                c = nextChar();
                if (isLetterLower(c)) { cur = c; state = 1; }
                else state = 2;                  /* cadeia vazia e valida */
                break;
            case 1:
                c = nextChar();
                if (isLetterLower(c) && c >= cur) { cur = c; state = 1; }
                else state = 2;                  /* nao-letra OU letra menor */
                break;
            case 2:
                retract();
                return ACEITA;
        }
    }
}

/* (e) comentario de bloco, sem terminador intercalado exceto entre aspas */
static int recE(void) {
    int state = 0;
    char c;
    while (1) {
        switch (state) {
            case 0:
                c = nextChar();
                if (c == '/') state = 1;
                else return fail();
                break;
            case 1:
                c = nextChar();
                if (c == '*') state = 2;
                else return fail();              /* nao e comentario (divisao) */
                break;
            case 2:                              /* corpo do comentario */
                c = nextChar();
                if (c == '\0')     return ERRO_LEX;   /* nao terminado */
                else if (c == '"') state = 5;
                else if (c == '*') state = 3;
                else               state = 2;
                break;
            case 3:                              /* viu *, sera o fim? */
                c = nextChar();
                if (c == '\0')     return ERRO_LEX;
                else if (c == '/') state = 4;    /* fecha! */
                else if (c == '*') state = 3;    /* outro *, segue candidato */
                else               state = 2;    /* era * isolado */
                break;
            case 5:                              /* dentro de "..." */
                c = nextChar();
                if (c == '\0')     return ERRO_LEX;
                else if (c == '"') state = 2;
                else               state = 5;
                break;
            case 4:
                return ACEITA;                   /* sem retract: pertence ao lexema */
        }
    }
}

/* (f) inteiros nas bases binaria, octal, hexadecimal e decimal */
enum { qF0 = 0, qB, qO, qD, qH, qBl, qACC, qERR };

static const char *baseNome = "";

static int recF(void) {
    int state = qF0;
    char c;
    while (1) {
        switch (state) {
            case qF0:
                c = nextChar();
                if (c >= '0' && c <= '1')      state = qB;
                else if (c >= '2' && c <= '7') state = qO;
                else if (c >= '8' && c <= '9') state = qD;
                else if (c >= 'A' && c <= 'F') state = qH;
                else return fail();
                break;

            case qB:                             /* so viu digitos 0-1 */
                c = nextChar();
                if (c >= '0' && c <= '1')      state = qB;
                else if (c >= '2' && c <= '7') state = qO;
                else if (c >= '8' && c <= '9') state = qD;
                else if (c == 'B')             state = qBl;  /* AMBIGUO */
                else if (c >= 'A' && c <= 'F') state = qH;
                else if (c == 'O') { baseNome = "OCTAL";   state = qACC; }
                else if (c == 'H') { baseNome = "HEX";     state = qACC; }
                else { baseNome = "DECIMAL"; retract(); state = qACC; }
                break;

            case qO:                             /* viu algum digito 2-7 */
                c = nextChar();
                if (c >= '0' && c <= '7')      state = qO;
                else if (c >= '8' && c <= '9') state = qD;
                else if (c >= 'A' && c <= 'F') state = qH;
                else if (c == 'O') { baseNome = "OCTAL"; state = qACC; }
                else if (c == 'H') { baseNome = "HEX";   state = qACC; }
                else { baseNome = "DECIMAL"; retract(); state = qACC; }
                break;

            case qD:                             /* viu 8 ou 9 */
                c = nextChar();
                if (isDigitC(c))               state = qD;
                else if (c >= 'A' && c <= 'F') state = qH;
                else if (c == 'H') { baseNome = "HEX"; state = qACC; }
                else { baseNome = "DECIMAL"; retract(); state = qACC; }
                break;

            case qH:                             /* viu A-F: so pode ser hex */
                c = nextChar();
                if (isHexDigit(c))  state = qH;
                else if (c == 'H') { baseNome = "HEX"; state = qACC; }
                else { retract(); state = qERR; } /* hex sem sufixo H */
                break;

            case qBl:                            /* leu [01]+ B : sufixo ou digito? */
                c = nextChar();
                if (c == 'H')            { baseNome = "HEX"; state = qACC; }
                else if (isHexDigit(c))  state = qH;          /* B era digito */
                else { baseNome = "BINARIO"; retract(); state = qACC; }
                break;

            case qACC: return ACEITA;
            case qERR: return ERRO_LEX;
        }
    }
}

/* (g) identificador: comeca por letra, nao pode terminar com _ */
static int recG(void) {
    int state = 0, pending = 0;
    char c;
    while (1) {
        switch (state) {
            case 0:
                c = nextChar();
                if (isLetterC(c)) state = 1;
                else return fail();
                break;
            case 1:                              /* ultimo char foi letra/digito */
                c = nextChar();
                if (isAlnumC(c))       state = 1;
                else if (c == '_')   { pending = 1; state = 2; }
                else                   state = 3;
                break;
            case 2:                              /* ultimo char foi _ */
                c = nextChar();
                if (c == '_')        { pending++;  state = 2; }
                else if (isAlnumC(c)){ pending = 0; state = 1; } /* _ confirmados */
                else                   state = 3;
                break;
            case 3:
                retractN(pending + 1);           /* lookahead + os _ pendentes */
                return ACEITA;
        }
    }
}

/* harness de teste */
static void teste(const char *rotulo, int (*rec)(void), const char *entrada) {
    char lexema[128];
    inputInit(entrada);
    baseNome = "";
    int r = rec();

    printf("  %-6s entrada=%-18s ", rotulo, entrada);
    if (r == ACEITA) {
        getLexeme(lexema, sizeof lexema);
        printf("ACEITA  lexema=\"%s\"", lexema);
        if (baseNome[0]) printf(" [%s]", baseNome);
        if (forward < inputLen) printf("  resta=\"%s\"", input + forward);
    } else if (r == ERRO_LEX) {
        printf("ERRO LEXICO");
    } else {
        printf("rejeita (fail: apontador voltou a %d)", forward);
    }
    printf("\n");
}

int main(void) {
    printf("(a) no minimo um 0\n");
    teste("ok",   recA, "1010+");
    teste("ok",   recA, "0");
    teste("falha",recA, "111");

    printf("\n(b) numero de 0's multiplo de 3\n");
    teste("ok",   recB, "000;");
    teste("ok",   recB, "1010101;");
    teste("ok",   recB, "111;");
    teste("falha",recB, "00;");

    printf("\n(c) cinco vogais em ordem\n");
    teste("ok",   recC, "aeiou.");
    teste("ok",   recC, "abcedefghioqrstuvw.");
    teste("falha",recC, "aeoiu.");

    printf("\n(d) ordem lexicografica crescente\n");
    teste("ok",   recD, "abc.");
    teste("ok",   recD, "aabbcc.");
    teste("corta",recD, "abcab");
    teste("corta",recD, "ba");

    printf("\n(e) comentario de bloco\n");
    teste("ok",   recE, "/* hello */x");
    teste("ok",   recE, "/* a * b */");
    teste("ok",   recE, "/* \"abc */ d\" */");
    teste("ok",   recE, "/*** oi ***/");
    teste("erro", recE, "/* sem fim");
    teste("falha",recE, "/ 2");

    printf("\n(f) inteiros com base\n");
    teste("ok",   recF, "1010B;");
    teste("ok",   recF, "725O;");
    teste("ok",   recF, "4096;");
    teste("ok",   recF, "1A3FH;");
    teste("ambig",recF, "1010BH;");
    teste("erro", recF, "1A3F;");

    printf("\n(g) identificador\n");
    teste("ok",   recG, "x=1");
    teste("ok",   recG, "media_final;");
    teste("ok",   recG, "a__b;");
    teste("corta",recG, "vetor__+");
    teste("falha",recG, "_erro");

    return 0;
}
