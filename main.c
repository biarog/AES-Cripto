#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "AES.h"

int main() {

    // INPUT: 
    //  - texto claro: string
    //  - chave: string

    unsigned char plain_text[16+1], key[16+1], cypher[16+1];
    int choice = 0;

    printf("============================================================================\n");
    printf("=               Bem vindo(a) ao programa de criptografia AES!!             =\n");
    printf("============================================================================\n");

    printf("Por favor, escolha se sua criptografia receberá um texto com caracteres \n'normais' ou em hexadecimal:\n");
    printf("normal = 0 | hex = 1\n");

    scanf(" %d", &choice);

    if(!choice) {
        printf("Digite seu texto claro: ");
        for (int i = 0; i < 16; i++){
            scanf(" %c", &plain_text[i]);
        }

    } else {
        printf("Digite seu texto claro em hexadecimal: ");
        for (int i = 0; i < 16; i++){
            scanf(" %2hhx", &plain_text[i]);
        }
    }
    
    plain_text[16] = '\0';

    printf("Digite a chave que será utilizada para criptografar em hex: ");
    for (int i = 0; i < 16; i++){
        scanf(" %2hhx", &key[i]);
    }
    key[16] = '\0';
    
    encrypt(key, plain_text, cypher);
    
    printf("\nTexto cifrado: ");
    for (int i = 0; i < 16; i++){ 
        printf("%02x", cypher[i]);
    }
    printf("\n");
    return 0;
}