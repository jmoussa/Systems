all: main 

main: count.o main.c
	gcc -g -Wall main.c count.o -o main

count.o: count.c count.h
	gcc -g -Wall -c count.c

clean:
	rm -rf *.o main *.exe
