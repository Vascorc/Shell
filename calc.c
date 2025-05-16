#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define epsilon 1.0e-16

void calc(char *value1, char *op, char *value2) {
    // Convertendo os operandos de string para double
    double num1 = atof(value1);
    double num2 = atof(value2);
    double result;

    // Checando a operação a ser realizada
    if (strcmp(op, "+") == 0) {
        result = num1 + num2;
    } 
    else if (strcmp(op, "-") == 0) {
        result = num1 - num2;
    } 
    else if (strcmp(op, "*") == 0) {
        result = num1 * num2;
    } 
    else if (strcmp(op, "/") == 0) {
        // Verificando divisão por zero
        if (fabs(num2) < epsilon) {
            printf("Erro: Divisão por zero!\n");
            return;
        }
        result = num1 / num2;
    } 
    else if (strcmp(op, "^") == 0) {
        result = pow(num1, num2);
    } 
    else {
        printf("Erro: Operador inválido '%s'\n", op);
        return;
    }

    // Imprimindo o resultado
    printf("Resultado calc %.3f %s %.3f = %.3f\n", num1, op, num2, result);
}

