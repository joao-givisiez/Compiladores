# Compiladores

Códigos desenvolvidos nas etapas da disciplina de **Compiladores** (Ciência da Computação — UFLA): analisadores léxicos com **Flex**, analisadores sintáticos LALR(1) com **Bison** e um analisador léxico implementado manualmente a partir de diagramas de transição em **C**.

---

## 📁 Estrutura

```
Compiladores/
├── Lista 1/                 # Analisadores léxicos (Flex)
│   ├── a_reais.l            # Reconhecimento de números reais (sinal, fração e expoente)
│   ├── b_esconder.l         # Oculta (troca por 'x') o texto entre <ESCONDER> e </ESCONDER>
│   └── c_tokens.l           # Classificação de tokens (palavras reservadas, tipos, funções,
│                            #   identificadores, strings, números, operadores e comentários)
├── Lista 2/lista2-impl/     # Analisadores sintáticos (Flex + Bison)
│   ├── q10a/                # Gramática A: S -> ( L ) | a ;  L -> L , S | S
│   ├── q10b/                # Gramática B: expressões booleanas (or, and, not, true, false)
│   ├── q11/                 # Declarações de variáveis em Pascal (simplificado)
│   └── q12/                 # Q11 + tipo array do Pascal (array [n..m] of T)
│       ├── main.l           # Scanner (Flex)
│       ├── main.y           # Parser (Bison)
│       ├── compiler.sh      # Script de compilação e execução
│       └── testes/          # Entradas válidas e inválidas
├── diagramas.c              # Analisador léxico manual baseado em diagramas de transição
└── LICENSE
```

---

## ⚙️ Pré-requisitos

- `flex`
- `bison`
- `gcc`

```bash
# Debian/Ubuntu
sudo apt install flex bison gcc
```

---

## ▶️ Como executar

### Lista 1 — analisadores léxicos

```bash
cd "Lista 1"
flex c_tokens.l
gcc lex.yy.c -o c_tokens
./c_tokens entrada.txt   # ou: ./c_tokens < entrada.txt
```

### Lista 2 — analisadores sintáticos

Cada questão tem um `compiler.sh` que gera o parser (Bison) e o scanner (Flex), compila e executa com o arquivo de entrada:

```bash
cd "Lista 2/lista2-impl/q10a"
bash compiler.sh testes/valido.txt     # entrada aceita pela gramática
bash compiler.sh testes/invalido.txt   # entrada com erro sintático
```

### Diagramas de transição

```bash
gcc diagramas.c -o diagramas
./diagramas
```

---

## 🧠 Conceitos aplicados

- Expressões regulares e autômatos finitos na análise léxica
- Implementação manual de diagramas de transição (`InicioLexema`, `ApontadorAdiante`, retrocesso e `fail()`)
- Gramáticas livres de contexto e análise sintática LALR(1)
- Recursão à esquerda e precedência de operadores em gramáticas
- Tratamento de erros sintáticos com `yyerror` e número de linha (`yylineno`)

---

## 📄 Licença

Distribuído sob a licença presente no arquivo [LICENSE](LICENSE).
