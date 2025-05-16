#include "shell.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/resource.h>
#include <string.h>

int fd_is_valid(int fd) {
    return fcntl(fd, F_GETFD) != -1 || errno != EBADF;
}

void openfile(char *nome) {
    int fd = open(nome, O_RDONLY);
    if (fd >= 0)
        printf("Aberto %s para operações de leitura com descritor fd %d\n", nome, fd);
    else
        perror(nome);
}

void closefd(int fd) {
    if (close(fd) < 0)
        perror("Erro ao fechar descritor");
    else
        printf("Descritor %d fechado com sucesso\n", fd);
}

void readfile(int fd, int nbytes) {
    if (!fd_is_valid(fd)) {
        printf("Descritor de ficheiro inválido: %d\n", fd);
        return;
    }

    char buffer[nbytes];
    int lidos = read(fd, buffer, nbytes);

    if (lidos < 0) {
        perror("Erro ao ler do ficheiro");
        return;
    }

    printf("Conteúdo lido (ASCII):\n");
    for (int i = 0; i < lidos; i++) {
        if (buffer[i] >= 32 && buffer[i] <= 126)
            putchar(buffer[i]);
        else
            putchar('.');
    }
    putchar('\n');

    printf("Conteúdo lido (HEX):\n");
    for (int i = 0; i < lidos; i++)
        printf("%02x ", (unsigned char)buffer[i]);
    putchar('\n');
}

void fileinfo() {
    if (stdout) {
        int fd = fileno(stdout);
        printf("STDOUT está aberto : descritor %d\n", fd);
    } else {
        printf("STDOUT está fechado.\n");
    }

    struct rlimit lim;
    getrlimit(RLIMIT_NOFILE, &lim);
    printf("O processo pode ter até %ld ficheiros abertos\n", lim.rlim_cur);

    int abertos = 0;
    printf("Descritores de ficheiros abertos: ");
    for (int i = 0; i < (int)lim.rlim_cur; i++) {
        if (fd_is_valid(i)) {
            printf("%d ", i);
            abertos++;
        }
    }
    printf("\nTotal de ficheiros abertos: %d\n", abertos);
}
