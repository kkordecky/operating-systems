CC = gcc
CFLAGS = -Wall -O3

.PHONY: all clean

all: main child

main: main.c
	$(CC) $(CFLAGS) main.c -o main

child: child.c
	$(CC) $(CFLAGS) child.c -o child

clean:
	rm -f main child *.o