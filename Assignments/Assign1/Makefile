all: memgrind 

memgrind: mymalloc.o memgrind.c
	gcc -g -Wall memgrind.c mymalloc.o -o memgrind

mymalloc.o: mymalloc.c mymalloc.h
	gcc -g -Wall -c mymalloc.c

clean:
	rm -rf *.o memgrind *.exe
