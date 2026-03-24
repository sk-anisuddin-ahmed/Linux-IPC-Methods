CC = gcc
CFLAGS = -Wall

all: clean client_server

client_server: client_server.c
	$(CC) client_server.c -o client_server $(CFLAGS)

clean:
	rm -f client_server