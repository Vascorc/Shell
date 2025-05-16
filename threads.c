#include "shell.h"
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

void aviso(char *mesg, int tempo) {
    while (tempo > 0) {
        sleep(1);
        tempo--;
    }
    fprintf(stderr, "Aviso : %s\n", mesg);
}


void *avisowrapper(void *args) {
 aviso_t * ptr = (aviso_t *)args;
 aviso( ptr->msg, ptr->tempo ) ;
 free(ptr);
 return NULL;
}



extern char strings[100][130]; 
extern int k;

void socp1(const char *fonte, const char *destino, int buffsize) {
  int fd1 = open(fonte, O_RDONLY);
  if (fd1 < 0) { perror("open fonte"); return; }

  int fd2 = open(destino, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd2 < 0) { perror("open destino"); close(fd1); return; }

  char *buffer = malloc(buffsize);
  if (!buffer) { perror("malloc"); close(fd1); close(fd2); return; }

  int n;
  while ((n = read(fd1, buffer, buffsize)) > 0) {
    if (write(fd2, buffer, n) != n) {
      perror("write");
      break;
    }
  }

  close(fd1);
  close(fd2);
  free(buffer);

  time_t tempoAtual;
  time(&tempoAtual);
  char *diaHora = ctime(&tempoAtual);
  if (diaHora[strlen(diaHora) - 1] == '\n') diaHora[strlen(diaHora) - 1] = '\0';

  snprintf(strings[k++], 130, "%s %s", diaHora, destino);
}

void *socpwrapper(void *args) {
  copiar_t *dados = (copiar_t *)args;
  socp1(dados->fonte, dados->destino, dados->buffsize);
  free(dados);
  return NULL;
}


void infoCopias() {
  printf("Relatório de cópias:\n");
  for (int i = 0; i < k && i < 100; i++) {
    printf("%s\n", strings[i]);
  }
}