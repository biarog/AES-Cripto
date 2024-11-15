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

    printf("Informe se seu texto claro será dado em\n(0) caracteres ASCII\n(1) valores hexadecimais\n: ");

    scanf(" %d", &choice);

    if(!choice) {
        printf("Digite seu texto claro (ASCII): ");
        for (int i = 0; i < 16; i++){
            scanf(" %c", &plain_text[i]);
        }

    } else {
        printf("Digite seu texto claro (hex): ");
        for (int i = 0; i < 16; i++){
            scanf(" %2hhx", &plain_text[i]);
        }
    }
    
    plain_text[16] = '\0';

    printf("Digite sua chave (hex): ");
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