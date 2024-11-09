#include "AES.h"

#include <stdlib.h>
#include <stdint.h>

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

void mix_columns(T_block matrix, T_block state) {
  uint8_t temp0, temp1, temp2, temp3;

  
  for (int j = 0; j < 4; j++) {
    temp0 = mult_GF(matrix[0][j], state[j][0])^mult_GF(matrix[1][j], state[j][1])^mult_GF(matrix[2][j], state[j][2])^mult_GF(matrix[3][j], state[j][3]);
    temp1 = mult_GF(matrix[3][j], state[j][0])^mult_GF(matrix[0][j], state[j][1])^mult_GF(matrix[1][j], state[j][2])^mult_GF(matrix[2][j], state[j][3]);
    temp2 = mult_GF(matrix[2][j], state[j][0])^mult_GF(matrix[3][j], state[j][1])^mult_GF(matrix[0][j], state[j][2])^mult_GF(matrix[1][j], state[j][3]);
    temp3 = mult_GF(matrix[1][j], state[j][0])^mult_GF(matrix[2][j], state[j][1])^mult_GF(matrix[3][j], state[j][2])^mult_GF(matrix[0][j], state[j][3]);

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
T_column* g_function(T_column word3, int round) {
  T_column gword;

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

  return gword;
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
  T_column g_word = g_function(key[3], round);

  // realizando as operações de adição  
  add_word(key[0], g_word);
  add_word(key[1], key[0]);
  add_word(key[2], key[1]);
  add_word(key[3], key[2]);
}

