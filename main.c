#include "gwm.h"

int main() {
    Display* display = XOpenDisplay(NULL);
    if (!display) {
        fputs("Failed to open display.", stderr);
        exit(EXIT_FAILURE);
    }
    EventManager* eventManager = EventManager_new(display);
    EventManager_run(eventManager);
    eventManager = EventManager_free(eventManager);
    XCloseDisplay(display);
}
