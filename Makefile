main: main.o aes.o 
	gcc main.o aes.o -o main

main.o: main.c aes.h
	gcc -c main.c -o main.o

aes.o: aes.c aes.h
	gcc -c aes.c -o aes.o

clean:
	rm aes *.o