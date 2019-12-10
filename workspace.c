#include "gwm.h"

/* Private function declarations */
static void Workspace_tileWindows(Workspace* self);

/* Constructor */
Workspace* Workspace_new(Display* display) {
    Workspace* self = ecalloc(1, sizeof(*self));
    self->display = display;
    self->screen = DefaultScreen(display);
    self->nWindows = 0;
    for (unsigned long iWindow = 0; iWindow < MAX_WINDOWS; iWindow++) {
        self->windows[iWindow] = 0;
    }
    return self;
}

/* Destructor */
Workspace* Workspace_free(Workspace* self) {
    free(self);
    return NULL;
}

/* Member functions */
void Workspace_handleWindow(Workspace* self, Window window) {
    self->windows[self->nWindows] = window;
    self->nWindows++;

    unsigned int monitorWidth = (unsigned int)DisplayWidth(self->display, self->screen);
    unsigned int monitorHeight = (unsigned int)DisplayHeight(self->display, self->screen);
    XMoveResizeWindow(self->display, window, 0, 0, monitorWidth, monitorHeight);
    XSelectInput(self->display, window,
        EnterWindowMask|
        FocusChangeMask|
        KeyPressMask|
        StructureNotifyMask
    );
    XMapWindow(self->display, window);
    XSetInputFocus(self->display, window, RevertToPointerRoot, CurrentTime);

    Workspace_tileWindows(self);
    Workspace_printWindowList(self);
}

void Workspace_focusWindow(Workspace* self, Window window) {
    XSetInputFocus(self->display, window, RevertToPointerRoot, CurrentTime);
}

void Workspace_hideAllWindows(Workspace* self) {
    int monitorWidth = DisplayWidth(self->display, self->screen);
    for (unsigned long iWindow = 0; iWindow < self->nWindows; iWindow++) {
        XMoveWindow(self->display, self->windows[iWindow], -2*monitorWidth, 0);
    }
}

void Workspace_printWindowList(Workspace* self) {  /* DEBUG */
    printf("Window List: ");
    for (unsigned long iWindow = 0; iWindow < self->nWindows; iWindow++) {
        printf("%lu ", self->windows[iWindow]);
    }
    printf("\n");
}

void Workspace_unHandleWindow(Workspace* self, Window window) {
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
    Workspace_tileWindows(self);
    Workspace_printWindowList(self);
}

/* Private member functions */
static void Workspace_tileWindows(Workspace* self) {
    int monitorWidth = (int)DisplayWidth(self->display, self->screen);
    int monitorHeight = (int)DisplayHeight(self->display, self->screen);

    if (self->nWindows == 1) {
        XMoveResizeWindow(self->display, self->windows[self->nWindows - 1], 0, 0, monitorWidth, monitorHeight);
    }
    else if (self->nWindows > 1) {
        int masterWindowWidth = (int)(monitorWidth * MASTER_FACTOR);
        int stackWindowHeight = (int)(monitorHeight / (self->nWindows - 1));

        XMoveResizeWindow(self->display, self->windows[self->nWindows - 1], 0, 0, masterWindowWidth, monitorHeight);

        for (unsigned long iWindow = self->nWindows - 2; iWindow < self->nWindows; iWindow--) {
            int x1 = masterWindowWidth + 1;
            int y1 = stackWindowHeight * (self->nWindows - (iWindow + 2)) + 1;
            int x2 = monitorWidth;
            int y2 = stackWindowHeight;
            XMoveResizeWindow(self->display, self->windows[iWindow], x1, y1, x2, y2);
        }
    }
}
