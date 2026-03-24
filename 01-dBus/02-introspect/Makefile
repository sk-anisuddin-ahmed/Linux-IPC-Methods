CC = gcc
CFLAGS = $(shell pkg-config --cflags dbus-1)
LIBS = $(shell pkg-config --libs dbus-1)

all: clean introspect-service

introspect-service: introspect-service.c
	$(CC) introspect-service.c -o introspect-service $(CFLAGS) $(LIBS)

clean:
	rm -f introspect-service