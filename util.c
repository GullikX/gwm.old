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

        int nSpecialTaskNames = sizeof(SPECIAL_TASK_NAMES) / sizeof(SPECIAL_TASK_NAMES[0]);
        for (int iSpecialTaskName = 0; iSpecialTaskName < nSpecialTaskNames; iSpecialTaskName++) {
            if (strcmp(taskName, SPECIAL_TASK_NAMES[iSpecialTaskName]) == 0) {
                chdir(SPECIAL_TASK_WORKDIRS[iSpecialTaskName]);
                break;
            }
        }

        setsid();
        execvp(((char **)cmd)[0], (char **)cmd);
    }
}

int xErrorHandler(Display* display, XErrorEvent* event) {
    char errorText[XERROR_BUFFER_SIZE];
    XGetErrorText(display, event->error_code, errorText, XERROR_BUFFER_SIZE);
    fprintf(stderr, "ERROR OCCURED: %s\n", errorText);
    fprintf(stderr, "    Display: %p\n", (void*)display);
    fprintf(stderr, "    XID resource id: %lu\n", event->resourceid);
    fprintf(stderr, "    Serial number of failed request: %lu\n", event->serial);
    fprintf(stderr, "    Error code of failed request: %u\n", (unsigned int)event->error_code);
    fprintf(stderr, "    Major op-code of failed request: %u\n", (unsigned int)event->request_code);
    fprintf(stderr, "    Minor op-code of failed request: %u\n", (unsigned int)event->minor_code);
    return 0;
}
