#include "AES.h"

#include <stdlib.h>
#include <stdint.h>

#include <stdio.h>

// TABELA DE SUBSTITUIÇÃO
// manter em um vetor (ao invés de matrix bidimensional) facilita a substiuição de bytes
// a valor de substiuição de xy encontra-se na xy-ésima posição do vetor
const uint8_t SBOX[] = {
/*         0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f */
/* 0 */  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
/* 1 */  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
/* 2 */  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
/* 3 */  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
/* 4 */  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
/* 5 */  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
/* 6 */  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
/* 7 */  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
/* 8 */  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
/* 9 */  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
/* a */  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
/* b */  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
/* c */  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
/* d */  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
/* e */  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
/* f */  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16,
};

// COEFICIENTES RC PARA AS 10 RODADAS
const uint8_t RC[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

const T_block CONST_M = {{0x02, 0x01, 0x01, 0x03}, {0x03, 0x02, 0x01, 0x01}, {0x01, 0x03, 0x02, 0x01}, {0x01, 0x01, 0x03, 0x02}};

// Transforma um string para um T_block
void string2tblock (char str[], T_block block) {
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            block[j][i] = str[4*j + i];
        }
    }
}

// CAMADA DE SUBSTITUIÇÃO
void sub_byte(T_block state) {
  uint8_t index;
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 4; i++) {
      index = state[j][i];
      state[j][i] = SBOX[index];
    }
  }
}

// CAMADA DE DIFUSÃO (deslocamento de linhas + embaralhamento de coluna)
void shift_rows(T_block state) {
  uint8_t temp;
  
  // shift na linha 0 -> deslocamento 0

  // shift na linha 1 -> deslocamento 1 pra esq
  temp = state[0][1];
  state[0][1] = state[1][1];
  state[1][1] = state[2][1];
  state[2][1] = state[3][1];
  state[3][1] = temp;

  // shift na linha 2 -> deslocamento 2 pra esq
  temp = state[0][2];
  state[0][2] = state[2][2];
  state[2][2] = temp;
  temp = state[1][2];
  state[1][2] = state[3][2];
  state[3][2] = temp;

  // shift na linha 3 -> deslocamento 3 pra esq == deslocamento 1 pra dir
  temp = state[3][3];
  state[3][3] = state[2][3];
  state[2][3] = state[1][3];
  state[1][3] = state[0][3];
  state[0][3] = temp;
}

uint8_t mult_GF(uint8_t a, uint8_t b) {
  uint8_t sum = 0, escapesField = 0; 

  // iteração sobre os 8 bits em b
  while (b) {

    // se o bit corrente (indicado sempre na posição menos significativa) estiver ativo 
    if(b & 1) {
      // o termo é somado ao resultado
      sum ^= a;
    }

    // checar fechamento (e realizar redução modular com polinômio irredutível)

    // como o uint8_t limita "a" em 8 bits, é necessário verificar/prever o overflow antes dele acontecer
    // indica se no próximo shift o valor de "a" escapa do fechamento
    escapesField = a & 0x80; // 80 hex = 10000000 bin

    a <<= 1;

    if (escapesField) {
      // como o bit que representa o x^8 no polinômio primo/irredutível sempre "cancela" o bit do overflow, 
      // ele pode ser omitido tendo em vista que o bit do overflow já foi descartado (pelo limite imposto pelo tipo uint8_t)
      a ^= 0x1b; // 1b hex = 0011011 bin =  x^4 + x^3 + x + 1 
    }

    // o shift representa justamente a iteração que atualiza o bit corrente 
    // e auxilia identificar se o bit corrente está ativo ou não
    b >>= 1;
  }

  return sum;
}

void mix_columns(const T_block matrix, T_block state) {
  uint8_t temp0, temp1, temp2, temp3;
  
  // percorre cada coluna do state e multiplica pela matriz constante
  for (int j = 0; j < 4; j++) {
    temp0 = mult_GF(matrix[0][0], state[j][0])^mult_GF(matrix[1][0], state[j][1])^mult_GF(matrix[2][0], state[j][2])^mult_GF(matrix[3][0], state[j][3]);
    temp1 = mult_GF(matrix[0][1], state[j][0])^mult_GF(matrix[1][1], state[j][1])^mult_GF(matrix[2][1], state[j][2])^mult_GF(matrix[3][1], state[j][3]);
    temp2 = mult_GF(matrix[0][2], state[j][0])^mult_GF(matrix[1][2], state[j][1])^mult_GF(matrix[2][2], state[j][2])^mult_GF(matrix[3][2], state[j][3]);
    temp3 = mult_GF(matrix[0][3], state[j][0])^mult_GF(matrix[1][3], state[j][1])^mult_GF(matrix[2][3], state[j][2])^mult_GF(matrix[3][3], state[j][3]);

    state[j][0] = temp0;
    state[j][1] = temp1;
    state[j][2] = temp2;
    state[j][3] = temp3;

  }
}

// ADIÇÃO DE CHAVE DA RODADA
void add_key(T_block state, T_block subkey) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      state[j][i] ^= subkey[j][i];
    }
  }  
}

// KEY SCHEDULE 
// FUNÇÃO G
void g_function(T_column gword, int round) {

  // deslocamento de 1 byte a esq
  uint8_t temp = gword[0];
  gword[0] = gword[1];
  gword[1] = gword[2];
  gword[2] = gword[3];
  gword[3] = temp;

  // substituição de byte com tabela SBOX
  uint8_t index;
  for (int i = 0; i < 4; i++) {
    index = gword[i];
    gword[i] = SBOX[index];
  }

  // adição de coeficiente
  gword[0] ^= RC[round];
}

// SOMA DAS PALAVRAS
// o primeiro parâmetro recebe o resultado
void add_word(T_column word1, T_column word2) {
  for (int i = 0; i < 4; i++) {
    word1[i] ^= word2[i];
  }
}

void key_schedule(T_block key, int round) {
  // passando a última coluna/word pra função g()
  T_column gword;
  for (int i = 0; i < 4; i++) {
    gword[i] = key[3][i];
  }
  g_function(gword, round);

  // realizando as operações de adição  
  add_word(key[0], gword);
  add_word(key[1], key[0]);
  add_word(key[2], key[1]);
  add_word(key[3], key[2]);
}

void print_block(T_block block) {
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      printf("[%02x]", block[j][i]);
    }
    printf("\n");
  }
}

void encrypt(unsigned char key[], unsigned char plain_txt[], unsigned char cypher[]) {
  T_block key_block, state;

  string2tblock(key, key_block);
  string2tblock(plain_txt, state);

  printf("ROUND 0: ===============\n");

  printf("\nstate:\n");
  print_block(state);

  printf("\nkey:\n");
  print_block(key_block);

  add_key(state, key_block);
  printf("\nAddRoundKey():\n");
  print_block(state);

  for (int i = 0; i < 10; i++) {
    printf("\n ROUND %d: ==============================", i+1);

    sub_byte(state);
    printf("\nstate pós SubBytes(): \n");
    print_block(state);

    shift_rows(state);
    printf("\nstate pós ShiftRows(): \n");
    print_block(state);

    if(i < 9) {
      mix_columns(CONST_M, state);
      printf("\nstate pós MixColumns(): \n");
      print_block(state);
    }

    key_schedule(key_block, i);
    printf("\nroundkey: \n");
    print_block(key_block);

    add_key(state, key_block);
    printf("\nstate pós AddRoundKey(): \n");
    print_block(state);
  }

  for(int j = 0; j < 4; j++) {
    for(int i = 0; i < 4; i++) {
      cypher[4*j + i] = state[j][i];
    } 
  }
  cypher[16] = '\0';
}