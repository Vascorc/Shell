// isjpg.c
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int isjpg(int fileDescriptor) {
    unsigned char b[4];
    int n = read(fileDescriptor, b, 4);  // Lê os primeiros 4 bytes

    if (n == 4) {
        // Verifica os magic bytes para JPG
        if (0xff == b[0] && 0xd8 == b[1] && 0xff == b[2] && (b[3] == 0xe0 || b[3] == 0xe1 || b[3] == 0xe2 || b[3] == 0xe8)) {
            lseek(fileDescriptor, 0, SEEK_SET);  // Volta ao início do arquivo
            return 1;  // É um arquivo JPG
        }
    }

    lseek(fileDescriptor, 0, SEEK_SET);  // Volta ao início do arquivo
    return 0;  // Não é um arquivo JPG
}
