CC = gcc
CFLAGS = -Wall -O3

all: main worker

main: main.o
	$(CC) main.o -o main

worker: worker.o integral.o
	$(CC) worker.o integral.o -o worker

main_fifo.o: main.c
	$(CC) $(CFLAGS) -c main_fifo.c

worker.o: worker.c
	$(CC) $(CFLAGS) -c worker.c

integral.o: integral.c
	$(CC) $(CFLAGS) -c integral.c

clean:
	rm -f *.o main worker integral 
	rm -f fifo_in 
	rm -f fifo_out