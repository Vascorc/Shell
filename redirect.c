#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define FILE_MODE 0644

// Função auxiliar fornecida
int dupandclose(int fd, int fdold, int numargs, char *args[]) {
    if (fd < 0 || fdold < 0) {
        perror("Invalid");
        exit(1);
    }
    dup2(fd, fdold);
    close(fd);
    args[numargs - 2] = NULL; // remove símbolo
    return numargs - 2;
}

int redirects(int numargs, char *args[]) {
    if (numargs >= 3 && strcmp(args[numargs - 2], "2>") == 0) {
        int fd = creat(args[numargs - 1], FILE_MODE);
        numargs = dupandclose(fd, STDERR_FILENO, numargs, args);
    }

    if (numargs >= 3 && strcmp(args[numargs - 2], ">") == 0) {
        int fd = creat(args[numargs - 1], FILE_MODE); // overwrite
        numargs = dupandclose(fd, STDOUT_FILENO, numargs, args);
    } else if (numargs >= 3 && strcmp(args[numargs - 2], ">>") == 0) {
        int fd = open(args[numargs - 1], O_CREAT | O_APPEND | O_WRONLY, FILE_MODE); // append
        numargs = dupandclose(fd, STDOUT_FILENO, numargs, args);
    }

    if (numargs >= 3 && strcmp(args[numargs - 2], "<") == 0) {
        int fd = open(args[numargs - 1], O_RDONLY);
        numargs = dupandclose(fd, STDIN_FILENO, numargs, args);
    }

    return numargs;
}