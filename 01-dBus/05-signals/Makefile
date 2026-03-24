CC = gcc
CFLAGS = $(shell pkg-config --cflags dbus-1)
LIBS = $(shell pkg-config --libs dbus-1)

all: clean signals-service

signals-service: signals-service.c
	$(CC) signals-service.c -o signals-service $(CFLAGS) $(LIBS)

clean:
	rm -f signals-service