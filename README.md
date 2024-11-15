# Criptografia AES
Trabalho desenvolvido para a disciplina de Introdução a Criptografia.

## Descrição
O arquivo `AES.c` implementa o algoritmo de cifragem que segue o Advanced Encryption Standard para chaves de 128 bits. Enquanto o `main.c`, que utiliza suas funções, é responsável por ler as entradas e exibir o texto cifrado. 
É importante apontar que o programa não implementa nenhum modo de operação ou tratamento de entradas e erros, por isso, o usuário deve informar os dados com precisamente 16 bytes.

## Execução 
Para executar, basta utilizar o script em `Makefile`:
```
make
```
e realizar sua chamada:
```
./main
```
Ou compilar e linkar manualmente os arquivos `AES.c` e `main.c`:
```
gcc aes.c main.c -o <programa>
```
e realizar sua chamada:
```
./<programa>
``` 

## Autores
Beatriz Rogers Tripoli Barbosa
Daniel Oliveira Rocha
Laura Naomi Seto
Raul Yoshiyuki Komai 