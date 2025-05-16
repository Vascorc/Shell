#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
typedef unsigned short tipo;


void bits(char *op1, char *op, char *op2) {
    unsigned short a = (unsigned short)atoi(op1);
    unsigned short b = (op2 != NULL) ? (unsigned short)atoi(op2) : 0;
    unsigned short result = 0;

    if (*op == '&') result = a & b;
    else if (*op == '|') result = a | b;
    else if (*op == '^') result = a ^ b;
    else if (*op == '~') result = ~a;
    else {
        printf("Operador inválido.\n");
        return;
    }

    if (*op == '~')
        printf("Resultado bits %s %s = %hu\n", op, op1, result);
    else
        printf("Resultado bits %hu %s %hu = %hu\n", a, op, b, result);
}

void printBits(tipo numero, tipo mascara) {
  while (mascara > 0) {
    putchar((numero & mascara) ? '1' : '0');
    mascara >>= 1;
  }
}

void printLinha(char *rotulo, tipo valor) {
  tipo mask = 0x8000;

  printf("%-15s ", rotulo);
  printBits(valor, mask);
  printf("  %5u  %5o  %3x\n", valor, valor, valor);
}

void displayBitOps(char *a1, char *a2) {
  tipo um = (tipo)atoi(a1);
  tipo dois = (tipo)atoi(a2);

  printf("%-15s %-18s %7s %7s %5s\n", "Expressão", "binary", "decimal", "octal", "hex");

  printLinha("um", um);
  printLinha("dois", dois);
  printLinha("um & dois", um & dois);
  printLinha("um | dois", um | dois);
  printLinha("um ^ dois", um ^ dois);
  printLinha("um &~ dois", um & ~dois);
}