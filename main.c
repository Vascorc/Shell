#include "shell.h"

char prompt[100];
char anterior[1024] = ""; // Diretório anterior para o comando "cd -"
char strings[100][130];
int k = 0;

int main()
{
  int len;
  int numargs;
  char linha[1024];
  char *args[64];

  strcpy(prompt, "SOSHELL: Introduza um comando : prompt>");
  while (1)
  {
    printf("%s", prompt);
    if (fgets(linha, 1023, stdin) == NULL)
    {
      printf("\n");
      exit(0);
    }
    len = strlen(linha);
    if (1 == len)
      continue; /* linha é apenas \n */
    if (linha[len - 1] == '\n')
      linha[len - 1] = '\0';
    numargs = parse(linha, args); /* particiona a string em argumentos */

    if (!builtin(args))
      execute(numargs, args); /* executa o comando */
  }
  return 0;
}

int builtin(char **args)
{
  /* Toda a funcionalidade embutida deverá devolver 1 */
  if (strcmp(args[0], "sair") == 0)
  {
    exit(0);
    return 1; /* funcionalidade embutida */
  }
  if (strncmp(args[0], "42", 2) == 0)
  {
    printf("42 is the answer to life the universe and everything\n");
    printf("https://pt.wikipedia.org/wiki/The_Hitchhiker's_Guide_to_the_Galaxy\n");
    return 1; /* funcionalidade embutida */
  }

  /* IMPORTANTE :
 Devolver 0 para indicar que não existe comando embutido e que
 será executado usando exec() na função execute.c
*/

  if (strcmp(args[0], "obterinfo") == 0)
  {
    printf("SoShell 2025 versão 1.0\n");
    return 1; // funcionalidade embutida
  }

  if (strlen(args[0]) > 4 && 0 == strncmp(args[0], "PS1=", 4))
  {
    strcpy(prompt, args[0] + 4);
    return 1; // comando embutido
  }

  if (0 == strcmp(args[0], "quemsoueu"))
  {
    system("id");
    return 1; // comando embutido
  }

  if (strcmp(args[0], "cd") == 0)
  {
    char atual[1024];
    if (getcwd(atual, sizeof(atual)) == NULL)
    {
      perror("getcwd");
      return 1;
    }

    int err;

    if (args[1] != NULL && strcmp(args[1], "-") == 0)
    {
      if (strlen(anterior) == 0)
      {
        printf("Não há diretório anterior.\n");
        return 1;
      }
      err = chdir(anterior);
    }
    else if (args[1] == NULL || strcmp(args[1], "~") == 0 || strcmp(args[1], "$HOME") == 0)
    {
      err = chdir(getenv("HOME"));
    }
    else
    {
      err = chdir(args[1]);
    }

    if (err < 0)
    {
      perror(args[1]);
    }
    else
    {
      strcpy(anterior, atual);
    }

    return 1;
  }

  if (strcmp(args[0], "socp") == 0)
  {
    if (args[1] != NULL && args[2] != NULL)
      socp(args[1], args[2]);
    else
      printf("Syntax Incorreto: Usage: socp fonte destino\n");
    return 1;
  }

  //-----P6----------

  if (strcmp(args[0], "calc") == 0)
  {
    if (args[1] != NULL && args[2] != NULL && args[3] != NULL)
    {
      calc(args[1], args[2], args[3]);
      return 1;
    }
    else
    {
      printf("Erro: Uso incorreto do comando calc.\n");
      return 1;
    }
  }

  if (strcmp(args[0], "bits") == 0)
  {
    int numArgs = 0;
    while (args[numArgs] != NULL)
      numArgs++; // Conta o número de argumentos

    // Caso com 3 argumentos: `bits 11 & 14`
    if (numArgs == 3 && strcmp(args[2], "&") == 0)
    {
      bits(args[1], args[2], NULL);
      return 1;
    }
    // Caso com 4 argumentos: `bits 8 ~`
    else if (numArgs == 4 && strcmp(args[3], "~") == 0)
    {
      bits(args[1], args[2], args[3]);
      return 1;
    }
    else
    {
      printf("Uso incorreto de bits. Ex: bits 11 & 14 ou bits 8 ~\n");
      return 1;
    }
  }

  // Verifica se o comando é "isjpg"
  if (strcmp(args[0], "isjpg") == 0)
  {
    if (args[1] != NULL)
    {
      int fd = open(args[1], O_RDONLY);
      if (fd == -1)
      {
        perror("Erro ao abrir o arquivo");
        return 1; // Se falhar ao abrir o arquivo
      }

      // Chama a função isjpg para verificar se é um JPG
      if (isjpg(fd))
      {
        printf("O arquivo '%s' é um JPG.\n", args[1]);
      }
      else
      {
        printf("O arquivo '%s' NÃO é um JPG.\n", args[1]);
      }

      close(fd); // Fecha o arquivo
    }
    else
    {
      printf("Uso incorreto do comando isjpg. Exemplo: isjpg <file>\n");
    }
    return 1; // Comando embutido
  }

  if (strcmp(args[0], "openfile") == 0 && args[1])
  {
    openfile(args[1]);
    return 1;
  }
  if (strcmp(args[0], "closefd") == 0 && args[1])
  {
    closefd(atoi(args[1]));
    return 1;
  }
  if (strcmp(args[0], "readfile") == 0 && args[1] && args[2])
  {
    readfile(atoi(args[1]), atoi(args[2]));
    return 1;
  }
  if (strcmp(args[0], "fileinfo") == 0)
  {
    fileinfo();
    return 1;
  }

  if (strcmp(args[0], "isValid") == 0 && args[1])
  {
    int fd = atoi(args[1]);
    printf("%d é %svalido\n", fd, fd_is_valid(fd) ? "" : "não ");
    return 1;
  }

  if (strcmp(args[0], "displayBitOps") == 0)
  {
    if (args[1] && args[2])
    {
      displayBitOps(args[1], args[2]);
    }
    else
    {
      printf("Uso correto: displayBitOps valor1 valor2\n");
    }
    return 1;
  }

  if (0 == strcmp(args[0], "avisoTeste"))
  {
    aviso(args[1], atoi(args[2]));
    return 1;
  }

  if (0 == strcmp(args[0], "aviso"))
  {
    pthread_t th;
    aviso_t *ptr = (aviso_t *)malloc(sizeof(aviso_t));
    strcpy(ptr->msg, args[1]);
    ptr->tempo = atoi(args[2]);
    pthread_create(&th, NULL, avisowrapper, (void *)ptr);
    return 1;
  }

  if (strcmp(args[0], "socpthread") == 0)
  {
    if (args[1] && args[2])
    {
      pthread_t th;
      copiar_t *info = malloc(sizeof(copiar_t));
      strcpy(info->fonte, args[1]);
      strcpy(info->destino, args[2]);
      info->buffsize = args[3] ? atoi(args[3]) : 1024;

      pthread_create(&th, NULL, socpwrapper, info);
      pthread_detach(th);
    }
    else
    {
      printf("Uso: socpthread <fonte> <destino> [bloco]\n");
    }
    return 1;
  }

  if (strcmp(args[0], "infoCopias") == 0)
  {
    infoCopias();
    return 1;
  }

  return 0;
}
