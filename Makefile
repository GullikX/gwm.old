CC=gcc
CFLAGS=-std=c99 -g -Og -pedantic-errors -Wall -Wextra -Wsign-conversion
CFILES=eventmanager.c main.c util.c workspace.c
LIBS=-lX11

gwm: $(CFILES)
	$(CC) $(CFLAGS) -o gwm $(CFILES) $(LIBS)

clean:
	rm -f gwm
