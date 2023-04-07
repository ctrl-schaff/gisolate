.POSIX:
.SUFFIXES:

CC = cc
CFLAGS = -W -O -g -Wall -Werror -Wextra -Wpedantic -Wconversion -Wcast-align -Wunused -Wshadow -Wpointer-arith
LDLIBS = -lm

all: gisolate
gisolate: gisolate.o set.o
	$(CC) $(LDFLAGS) -o gisolate gisolate.o set.o $(LDLIBS)
gisolate.o: gisolate.c gisolate.h set.h
	$(CC) -c $(CFLAGS) gisolate.c
set.o: set.c set.h
	$(CC) -c $(CFLAGS) set.c
clean:
	rm -f gisolate gisolate.o set.o
