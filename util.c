#include "gwm.h"

void* ecalloc(size_t nItems, size_t itemSize) {
    void* pointer = calloc(nItems, itemSize);
    if (!pointer) {
        fprintf(stderr, "ecalloc: Failed to allocate %zux%zu bytes.\n", nItems, itemSize);
        exit(EXIT_FAILURE);
    }
    return pointer;
}

void spawn(Display* display, const char* cmd[]) {
    if (fork() == 0) {
        if (display) {
            close(ConnectionNumber(display));
        }
        setsid();
        execvp(((char **)cmd)[0], (char **)cmd);
    }
}
