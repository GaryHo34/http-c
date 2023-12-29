# define the C compiler to use
CC = gcc

# define compile-time flags all errors, warnings, and debugging symbols
CFLAGS = -Wall -Werror -Wextra -g 

# define the linker flags
LDFLAGS = -lpthread

all: httpc

httpc: main.o server.o request.o threadp.o taskqueue.o
	$(CC) $(CFLAGS) -o httpc main.o server.o request.o threadp.o taskqueue.o $(LDFLAGS)

main.o: main.c server.h log.h
	$(CC) $(CFLAGS) -c main.c

server.o: server.c server.h log.h
	$(CC) $(CFLAGS) -c server.c

request.o: request.c request.h log.h
	$(CC) $(CFLAGS) -c request.c

threadp.o: threadp.c threadp.h 
	$(CC) $(CFLAGS) -c threadp.c

taskqueue.o: taskqueue.c taskqueue.h log.h
	$(CC) $(CFLAGS) -c taskqueue.c

clean:
	rm -f *.o httpc
