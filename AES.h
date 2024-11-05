#ifndef AES_H
#define AES_H

/* Função de substituição de bytes 
*  @param bytes Os bytes a serem substituidos
*  @return Os bytes que foram substituídos
*/
int s_table(int bytes);

/* Função de deslocamento de colunas 
*  @param state A matriz de bytes que vai sofrer o deslocamento
*/
void shift_rows(int state[][4]);

#endif