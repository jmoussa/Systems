all: client server 

client: client.c libnetfiles.o
	gcc -g -Wall client.c libnetfiles.o -o client

server: netfileserver.c
	gcc -g -Wall netfileserver.c -o server

libnetfiles.o: libnetfiles.c libnetfiles.h
	gcc -g -Wall -c libnetfiles.c

clean:
	rm -rf *.o client server *.exe
