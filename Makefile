CC=gcc
CFLAGS=-std=c99 -g -Og -pedantic-errors -Wall -Wextra -Wsign-conversion
CFILES=main.c task.c taskmanager.c util.c windowmanager.c workspace.c
LIBS=-lX11
PREFIX = /usr/local

gwm: $(CFILES)
	$(CC) $(CFLAGS) -o gwm $(CFILES) $(LIBS)

clean:
	rm -f gwm

install:
	@install -Dm 755 gwm "${DESTDIR}${PREFIX}/bin/gwm"
	@install -Dm 755 gwm-taskswitcher.sh "${DESTDIR}${PREFIX}/bin/gwm-taskswitcher"
