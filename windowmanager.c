#include "gwm.h"

/* Private function declarations */
static void WindowManager_printWindowList(WindowManager* self);
static void WindowManager_tileWindows(WindowManager* self);

/* Constructor */
WindowManager* WindowManager_new(Display* display) {
    WindowManager* self = ecalloc(1, sizeof(*self));
    self->display = display;
    self->screen = DefaultScreen(display);
    self->nWindows = 0;
    for (unsigned long iWindow = 0; iWindow < MAX_WINDOWS; iWindow++) {
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

    WindowManager_tileWindows(self);
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
    WindowManager_tileWindows(self);
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

static void WindowManager_tileWindows(WindowManager* self) {
    int monitorWidth = (int)DisplayWidth(self->display, self->screen);
    int monitorHeight = (int)DisplayHeight(self->display, self->screen);

    int masterX1;
    int masterY1;
    int masterX2;
    int masterY2;
    int stackYSize;

    if (self->nWindows == 1) {
        masterX1 = 0;
        masterY1 = 0;
        masterX2 = monitorWidth;
        masterY2 = monitorHeight;
        stackYSize = 0;
    }
    else {
        masterX1 = 0;
        masterY1 = 0;
        masterX2 = (int)(monitorWidth * MASTER_FACTOR);
        masterY2 = monitorHeight;
        stackYSize = (int)(monitorHeight / (self->nWindows - 1));
    }

    for (unsigned long iWindow = 0; iWindow < self->nWindows; iWindow++) {
        if (iWindow == 0) {
            XMoveResizeWindow(self->display, self->windows[iWindow], masterX1, masterY1, masterX2, masterY2);
        }
        else {
            int x1 = masterX2 + 1;
            int y1 = stackYSize * (iWindow - 1);
            int x2 = monitorWidth;
            int y2 = y1 + stackYSize;
            XMoveResizeWindow(self->display, self->windows[iWindow], x1, y1, x2, y2);
        }
    }
}
