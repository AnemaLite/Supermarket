output: main.o fonctions.o
	gcc -g -Wall main.o fonctions.o -o output

main.o: main.c
	gcc -c -g -Wall main.c

fonctions.o: fonctions.c fonctions.h
	gcc -c -g -Wall fonctions.c
clean:
	rm *.o