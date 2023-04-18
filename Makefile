# define the C compiler to use
CC = gcc

# define compile-time flags all errors, warnings, and debugging symbols
CFLAGS = -Wall -g

# define any directories containing header files other than /usr/include
INCLUDES = -I/usr/local/include

# define library paths in addition to /usr/lib
LFLAGS = -L/usr/local/lib

# define any libraries to link into executable:
LIBS = -lm

# define the C source files
SRCS = main.c

# define the C object files
OBJS = $(SRCS:.c=.o)

# define the executable file
MAIN = main

# define the header files
HDRS =

# define the dependencies
DEPS = $(HDRS) Makefile

# define the rules

all: $(MAIN)
$(MAIN): $(OBJS)
$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

.c.o:
$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
$(RM) $(OBJS) *~ $(MAIN)

# end of makefile