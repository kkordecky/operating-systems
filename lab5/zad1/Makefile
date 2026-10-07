CC = gcc

CFLAGS = -Wall -O3

OBJS = main.o producer.o consumer.o

.PHONY: all clean

all: app

app: $(OBJS)
	$(CC) $(OBJS) -pthread -o app

%.o: %.c common.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o app