CC = gcc
CFLAGS = $(shell pkg-config --cflags dbus-1)
LIBS = $(shell pkg-config --libs dbus-1)

all: clean ping-service

ping-service: ping-service.c
	$(CC) ping-service.c -o ping-service $(CFLAGS) $(LIBS)

clean:
	rm -f ping-service