CC = gcc
CFLAGS = $(shell pkg-config --cflags dbus-1)
LIBS = $(shell pkg-config --libs dbus-1)

all: clean hello-service

hello-service: hello-service.c
	$(CC) hello-service.c -o hello-service $(CFLAGS) $(LIBS)

clean:
	rm -f hello-service