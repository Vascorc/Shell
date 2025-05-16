#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "shell.h"

#define BUFSIZE 1024

void ioCopy(int IN, int OUT) {
    char buffer[BUFSIZE];
    ssize_t bytesRead, bytesWritten;

    while ((bytesRead = read(IN, buffer, BUFSIZE)) > 0) {
        bytesWritten = write(OUT, buffer, bytesRead);
        if (bytesWritten != bytesRead) {
            perror("Erro ao escrever no destino");
            break;
        }
    }
    if (bytesRead < 0)
        perror("Erro ao ler da fonte");
}

void socp(char *fonte, char *destino) {
    int inFD = open(fonte, O_RDONLY);
    if (inFD < 0) {
        perror(fonte);
        return;
    }

    int outFD = open(destino, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (outFD < 0) {
        perror(destino);
        close(inFD);
        return;
    }

    ioCopy(inFD, outFD);

    close(inFD);
    close(outFD);
}
