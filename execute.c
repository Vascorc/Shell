#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell.h"

void execute(int numargs, char **args) {
    int pid, status;
    int code = ultimo(&numargs, args);

    // Verificar se há pipe
    int pipe_index = -1;
    for (int i = 0; i < numargs; i++) {
        if (strcmp(args[i], "|") == 0) {

            pipe_index = i;
            break;
        }
    }

    if (pipe_index == -1) {
        // Sem pipe
        if ((pid = fork()) < 0) {
            perror("fork");
            exit(1);
        }

        if (pid == 0) {
            numargs = redirects(numargs, args);
            if (numargs == -1) {
                fprintf(stderr, "Erro no redirecionamento\n");
                exit(1);
            }
            execvp(*args, args);
            perror(*args);
            exit(1);
        }

        if (code == FG) {
            while (wait(&status) != pid);
        }
        return;
    }

    // Com pipe
    args[pipe_index] = NULL; // separa os dois comandos
    char **left = args;
    char **right = &args[pipe_index + 1];

    int fd[2];
    if (pipe(fd) < 0) {
        perror("pipe");
        exit(1);
    }

    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        // Processo filho
        int pid2 = fork();

        if (pid2 < 0) {
            perror("fork");
            exit(1);
        }

        if (pid2 == 0) {
            // Neto: comando da direita
            close(fd[1]);
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);

            int rargs = 0;
            while (right[rargs]) rargs++;
            rargs = redirects(rargs, right);
            if (rargs == -1) {
                fprintf(stderr, "Erro no redirecionamento\n");
                exit(1);
            }

            execvp(right[0], right);
            perror(right[0]);
            exit(1);
        } else {
            // Filho: comando da esquerda
            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);

            int largs = pipe_index;
            largs = redirects(largs, left);
            if (largs == -1) {
                fprintf(stderr, "Erro no redirecionamento\n");
                exit(1);
            }

            execvp(left[0], left);
            perror(left[0]);
            exit(1);
        }
    }

    // Processo pai
    close(fd[0]);
    close(fd[1]);
    if (code == FG) {
        waitpid(pid, &status, 0);
    }
}

int ultimo(int *numargs, char **args) {
    if (args[*numargs - 1][0] == '&') {
        *numargs = *numargs - 1;
        args[*numargs] = NULL;
        return BG;
    }
    return FG;
}