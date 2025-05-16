# Compilador e flags
CC = cc
CFLAGS = -Wall -c
LIBS = -lm

# Objetos do projeto (agora com threads.o incluído)
OBJS = main.o execute.o parse.o socp.o isjpg.o calc.o bits.o files.o redirect.o threads.o

# Alvo padrão: compilar o executável soshell
all: soshell

# Regras de compilação para cada .c
main.o: main.c shell.h
	$(CC) $(CFLAGS) main.c

execute.o: execute.c shell.h
	$(CC) $(CFLAGS) execute.c

parse.o: parse.c shell.h
	$(CC) $(CFLAGS) parse.c

socp.o: socp.c shell.h
	$(CC) $(CFLAGS) socp.c

isjpg.o: isjpg.c shell.h
	$(CC) $(CFLAGS) isjpg.c

calc.o: calc.c shell.h
	$(CC) $(CFLAGS) calc.c

bits.o: bits.c shell.h
	$(CC) $(CFLAGS) bits.c

files.o: files.c shell.h
	$(CC) $(CFLAGS) files.c

redirect.o: redirect.c shell.h redirect.h
	$(CC) $(CFLAGS) -o redirect.o redirect.c

threads.o: threads.c shell.h
	$(CC) $(CFLAGS) threads.c

# Linkagem final
soshell: $(OBJS)
	$(CC) -o soshell $(OBJS) $(LIBS)

# Limpeza
clean:
	rm -f soshell *.o *~
	@echo "Remover: Ficheiros executáveis, objetos e temporários."
