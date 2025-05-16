#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include <sys/wait.h>
#include "redirect.h"

typedef struct { char msg[100] ; int tempo;} aviso_t;

typedef struct {
  char fonte[100];
  char destino[100];
  int buffsize;
} copiar_t;



int parse(char *buf, char **args);

void execute(int numargs, char **args);

int builtin (char **args);

void socp( char *fonte,  char *destino);
void ioCopy(int IN,int OUT);

int ultimo(int *numargs, char **args);

void calc(char *value1, char *op, char *value2);
void bits(char *arg1, char *op, char *arg2);
int isjpg(int fileDescriptor);

int fd_is_valid(int fd);
void openfile(char * nome);
void closefd(int fd);
void readfile(int fd, int nbytes);
void fileinfo();

void displayBitOps(char *a1, char *a2);


int containsPipe(int numArgs, char **args);

void aviso(char *mesg, int tempo);
void *avisowrapper(void *args);

void *socpwrapper(void *args);
void socp1(const char *fonte, const char *destino, int buffsize);
void infoCopias();


/* constantes que podem tornar uteis*/
extern char strings[100][130];
extern int k;

#define BG 0
#define FG 1

/* constantes que podem tornar uteis*/

#define BG 0
#define FG 1
