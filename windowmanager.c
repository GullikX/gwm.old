#include "gwm.h"

/* Private function declarations */
static void WindowManager_printWindowList(WindowManager* self);

/* Constructor */
WindowManager* WindowManager_new(Display* display) {
    WindowManager* self = ecalloc(1, sizeof(*self));
    self->display = display;
    self->screen = DefaultScreen(display);
    self->nWindows = 0;
    for (unsigned long iWindow = 0; iWindow < 256; iWindow++) {
        self->windows[iWindow] = 0;
    }
    return self;
}

/* Destructor */
WindowManager* WindowManager_free(WindowManager* self) {
    free(self);
    return NULL;
}

/* Member functions */
void WindowManager_handleWindow(WindowManager* self, Window window) {
    self->windows[self->nWindows] = window;
    self->nWindows++;

    unsigned int monitorWidth = (unsigned int)DisplayWidth(self->display, self->screen);
    unsigned int monitorHeight = (unsigned int)DisplayHeight(self->display, self->screen);
    XMoveResizeWindow(self->display, window, 0, 0, monitorWidth, monitorHeight);
    XSelectInput(self->display, window,
        EnterWindowMask|
        FocusChangeMask|
        StructureNotifyMask
    );
    XMapWindow(self->display, window);
    XSetInputFocus(self->display, window, RevertToPointerRoot, CurrentTime);

    WindowManager_printWindowList(self);
}

void WindowManager_unHandleWindow(WindowManager* self, Window window) {
    unsigned long iWindow;
    for (iWindow = 0; iWindow < self->nWindows; iWindow++) {
        if (self->windows[iWindow] == window) {
            self->windows[iWindow] = 0;
            break;
        }
    }
    if (iWindow < self->nWindows) {
        self->nWindows--;
        for (unsigned long iSubsequentWindow = iWindow; iSubsequentWindow < self->nWindows; iSubsequentWindow++) {
            self->windows[iSubsequentWindow] = self->windows[iSubsequentWindow + 1];
        }
    }
    WindowManager_printWindowList(self);
}

/* Private member functions */
static void WindowManager_printWindowList(WindowManager* self) {
    printf("Window List: ");
    for (unsigned long iWindow = 0; iWindow < self->nWindows; iWindow++) {
        printf("%lu ", self->windows[iWindow]);
    }
    printf("\n");
}
