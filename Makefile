CC=gcc
CFLAGS=-std=c99 -g -Og -pedantic-errors -Wall -Wextra -Wsign-conversion
CFILES=main.c

gwm: $(CFILES)
	$(CC) $(CFLAGS) -o gwm $(CFILES)
