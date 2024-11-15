#ifndef AES_H
#define AES_H

#include <stdlib.h>
#include <stdint.h>

// a separação do bloco em colunas facilita o mix de colunas 
// essa configuração do bloco implica em um acesso aos elementos dado por: state[col][row] 
// como o AES implementado é o de 128 bits, a utilização do tipo uint8_t é conveniente
typedef uint8_t T_column[4];
typedef T_column T_block[4];

void string2tblock (char str[], T_block block);

void sub_byte(T_block state);

void shift_rows(T_block state);

uint8_t mult_GF(uint8_t a, uint8_t b);

void mix_columns(const T_block state, T_block matriz);

void add_key(T_block state, T_block subkey);

void g_function(T_column gword, int round);

void add_word(T_column word1, T_column word2);

void key_schedule(T_block key, int round);

void encrypt(unsigned char key[], unsigned char plain_txt[], unsigned char cypher[]);

#endif