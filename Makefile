# define the C compiler to use
CC = gcc

# define compile-time flags all errors, warnings, and debugging symbols
CFLAGS = -Wall -g

all: httpc

httpc: main.o server.o request.o
	$(CC) $(CFLAGS) -o httpc main.o server.o request.o

main.o: main.c server.h log.h
	$(CC) $(CFLAGS) -c main.c

server.o: server.c server.h log.h
	$(CC) $(CFLAGS) -c server.c

request.o: request.c request.h log.h
	$(CC) $(CFLAGS) -c request.c

clean:
	rm -f *.o httpc
