#!/bin/bash
# Compila o parser (Bison) e o scanner (Flex) e executa com o arquivo de entrada
# Uso (dentro da pasta da questao): bash compiler.sh <arquivo_de_entrada>
bison -d -o parser.tab.c main.y      # gera parser.tab.c e parser.tab.h
flex -o scanner.yy.c main.l          # gera scanner.yy.c
gcc -o main parser.tab.c scanner.yy.c -lfl
./main < "$1"
