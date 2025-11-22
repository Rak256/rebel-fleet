CC = gcc
CFLAGS = -std=c99 -g -D_POSIX_C_SOURCE=200809L

default: all

all: server
server: rebel-fleet.c
	$(CC) $< $(CFLAGS) -o rebel-fleet

clean:
	rm -f rebel-fleet
