#include "gwm.h"

int main() {
    puts("This is gwm "VERSION);
    Display* display = XOpenDisplay(NULL);
    if (!display) {
        fputs("Failed to open display.", stderr);
        exit(EXIT_FAILURE);
    }
    WindowManager* windowManager = WindowManager_new(display);
    WindowManager_run(windowManager);
    windowManager = WindowManager_free(windowManager);
    XCloseDisplay(display);
}
