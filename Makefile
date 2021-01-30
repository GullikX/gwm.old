CC?=cc
CFLAGS?=-O2

VERSION=0.1.2

WARNINGS=-pedantic -Wall -Wextra -Wunused-const-variable
ERRORS=-Werror=vla -Werror=strict-prototypes -Werror=implicit-fallthrough
DEFINES=-DVERSION=\"${VERSION}\"
OPTS=-std=c99 $(WARNINGS) $(ERRORS) $(DEFINES)

LIBS=-lX11
PREFIX=/usr/local

gwm: gwm.c config.h
	$(CC) $(CFLAGS) $(OPTS) -o gwm $< $(LIBS)

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

.PHONY: cppcheck
cppcheck: gwm.c
	cppcheck --enable=all $<
