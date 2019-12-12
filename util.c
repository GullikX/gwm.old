#include "gwm.h"

void* ecalloc(size_t nItems, size_t itemSize) {
    void* pointer = calloc(nItems, itemSize);
    if (!pointer) {
        fprintf(stderr, "ecalloc: Failed to allocate %zux%zu bytes.\n", nItems, itemSize);
        exit(EXIT_FAILURE);
    }
    return pointer;
}

int modulo(int a, int b) {
    return (a % b + b) % b;
}

void spawn(const char* cmd[], Display* display, const char* taskName) {
    if (fork() == 0) {
        if (display) {
            close(ConnectionNumber(display));
        }

        for (int iSpecialTaskName = 0; iSpecialTaskName < LENGTH(SPECIAL_TASK_NAMES); iSpecialTaskName++) {
            if (strcmp(taskName, SPECIAL_TASK_NAMES[iSpecialTaskName]) == 0) {
                chdir(SPECIAL_TASK_WORKDIRS[iSpecialTaskName]);
                break;
            }
        }

        setsid();
        execvp(((char **)cmd)[0], (char **)cmd);
    }
}
