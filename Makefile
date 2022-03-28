.POSIX:
.SUFFIXES:

CC = cc
CFLAGS = -W -O -g -Wall -Werror -Wextra -Wpedantic -Wconversion -Wcast-align -Wunused -Wshadow -Wpointer-arith
LDLIBS = -lm

all: gisolate
gisolate: gisolate.o
	$(CC) $(LDFLAGS) -o gisolate gisolate.o $(LDLIBS)
gisolate.o: gisolate.c 
	$(CC) -c $(CFLAGS) gisolate.c
clean:
	rm -f gisolate gisolate.o
	sudo rm -rf ./build
