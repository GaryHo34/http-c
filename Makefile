# define the C compiler to use
CC = gcc

# define compile-time flags all errors, warnings, and debugging symbols
CFLAGS = -Wall -g

all: main

main: main.o httpc.o
	$(CC) $(CFLAGS) -o main main.o httpc.o

main.o: main.c httpc.h
	$(CC) $(CFLAGS) -c main.c

httpc.o: httpc.c httpc.h
	$(CC) $(CFLAGS) -c httpc.c

clean:
	rm -f *.o main
