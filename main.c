#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "AES.h"

int main() {

    char key[16+1], plain_txt[16+1];
    char cypher[16+1];

    scanf("Digite o texto a ser cifrado: %[^\n]", plain_txt);
    scanf("Digite a chave a ser utilizada na cifra: %[^\n]", key);

    encrypt(key, plain_txt, cypher);

    printf("Sucesso!\nO seu texto cifrado é:\n");

    for(int i = 0; i < 17; i++) {
        printf("%x", cypher[i]);
    }
    printf("\n");

    return 0;
}