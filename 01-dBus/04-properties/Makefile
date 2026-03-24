CC = gcc
CFLAGS = $(shell pkg-config --cflags dbus-1)
LIBS = $(shell pkg-config --libs dbus-1)

all: clean properties-service

properties-service: properties-service.c
	$(CC) properties-service.c -o properties-service $(CFLAGS) $(LIBS)

clean:
	rm -f properties-service