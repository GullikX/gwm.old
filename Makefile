CC=gcc
#CFLAGS=-std=c99 -g -Og -pedantic-errors -Wall -Wextra -Wno-unused-variable
CFLAGS=-std=c99 -O2 -pedantic-errors -Wall -Wextra -Wno-unused-variable
CFILES=main.c task.c taskmanager.c util.c windowmanager.c workspace.c
LIBS=-lX11
PREFIX = /usr/local
VERSION = 0.0.2

gwm: $(CFILES) config.h
	$(CC) -DVERSION=\"${VERSION}\" $(CFLAGS) -o gwm $(CFILES) $(LIBS)

config.h:
	cp config.def.h $@

clean:
	rm -f gwm

install:
	install -Dm 755 gwm "${DESTDIR}${PREFIX}/bin/gwm"
	install -Dm 755 gwm-taskswitcher.sh "${DESTDIR}${PREFIX}/bin/gwm-taskswitcher"

uninstall:
	rm -f "${DESTDIR}${PREFIX}/bin/gwm"
	rm -f "${DESTDIR}${PREFIX}/bin/gwm-taskswitcher"
