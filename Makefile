CC=gcc
#CFLAGS=-std=c99 -g -Og -pedantic -Wall -Wextra -Wno-unused-variable -Werror=vla -Werror=strict-prototypes -Werror=implicit-fallthrough
CFLAGS=-std=c99 -O2 -pedantic -Wall -Wextra -Wno-unused-variable -Werror=vla -Werror=strict-prototypes -Werror=implicit-fallthrough
CFILES=main.c task.c taskmanager.c util.c windowmanager.c workspace.c
LIBS=-lX11
PREFIX=/usr/local
VERSION=0.0.3-git

gwm: $(CFILES) config.h
	$(CC) -DVERSION=\"${VERSION}\" $(CFLAGS) -o gwm $(CFILES) $(LIBS)

config.h:
	cp config.def.h $@

.PHONY: clean
clean:
	rm -f gwm

.PHONY: install
install:
	install -Dm 755 gwm "${DESTDIR}${PREFIX}/bin/gwm"
	install -Dm 755 gwm-taskswitcher.sh "${DESTDIR}${PREFIX}/bin/gwm-taskswitcher"

.PHONY: uninstall
uninstall:
	rm -f "${DESTDIR}${PREFIX}/bin/gwm"
	rm -f "${DESTDIR}${PREFIX}/bin/gwm-taskswitcher"
