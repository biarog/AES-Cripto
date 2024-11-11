#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "AES.h"

int main() {

    // INPUT: 
    //  - texto claro: string
    //  - chave: string

    unsigned char plain_text[16+1], key[16+1], cypher[16+1];

    printf("Texto claro: ");
    for (int i = 0; i < 16; i++){
        scanf("%2hhx", &plain_text[i]);
    }
    plain_text[16] = '\0';

    printf("Chave: ");
    for (int i = 0; i < 16; i++){
        scanf("%2hhx", &key[i]);
    }
    key[16] = '\0';

    encrypt(key, plain_text, cypher);

    return 0;
}