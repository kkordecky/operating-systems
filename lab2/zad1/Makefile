CC = gcc
CFLAGS = -Wall -O3
LDFLAGS =

SOURCES = main.c child.c
OBJECTS = $(SOURCES:.c=.o)

TARGET = main

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.c signals.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET)