CC=gcc
CFLAGS=-std=c99 -g -Og -pedantic-errors -Wall -Wextra -Wsign-conversion
CFILES=eventmanager.c main.c util.c

gwm: $(CFILES)
	$(CC) $(CFLAGS) -lX11 -o gwm $(CFILES)
