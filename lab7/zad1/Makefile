CC = gcc
CFLAGS = -Wall -O3

SERVER_OBJS = server.o server_utils.o server_handlers.o
CLIENT_OBJS = client.o client_utils.o

.PHONY: all clean

all: server client

server: $(SERVER_OBJS)
	$(CC) $(SERVER_OBJS) -o server -pthread

client: $(CLIENT_OBJS)
	$(CC) $(CLIENT_OBJS) -o client -pthread

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o server client