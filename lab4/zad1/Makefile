CC = gcc

CFLAGS = -Wall -O3

SERVER_OBJS = server.o server_utils.o
CLIENT_OBJS = client.o client_utils.o

.PHONY: all clean

# ===== build =====
all: server client

# ===== server =====
server: $(SERVER_OBJS)
	$(CC) $(SERVER_OBJS) -o server

# ===== client =====
client: $(CLIENT_OBJS)
	$(CC) $(CLIENT_OBJS) -o client

# ===== object files =====
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# ===== clean =====
clean:
	rm -f *.o server client