#include "gwm.h"

/* Constructor */
Workspace* Workspace_new(Display* display) {
    Workspace* self = ecalloc(1, sizeof(*self));
    self->display = display;
    self->screen = DefaultScreen(display);
    self->nWindows = 0;
    self->masterFactor = MASTER_FACTOR;
    for (unsigned long iWindow = 0; iWindow < MAX_WINDOWS_PER_WORKSPACE; iWindow++) {
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
void Workspace_adjustMasterFactor(Workspace* self, double amount) {
    self->masterFactor += amount;
    if (self->masterFactor > MASTER_FACTOR_MAX) self->masterFactor = MASTER_FACTOR_MAX;
    if (self->masterFactor < MASTER_FACTOR_MIN) self->masterFactor = MASTER_FACTOR_MIN;
    Workspace_tileWindows(self);
}

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
}

void Workspace_changeFocus(Workspace* self, int iOffset) {
    int iWindow = modulo(self->iWindowFocused + iOffset, self->nWindows);
    Workspace_focusWindow(self, self->windows[iWindow]);
}

void Workspace_focusWindow(Workspace* self, Window window) {
    for (unsigned long iWindow = 0; iWindow < self->nWindows; iWindow++) {
        if (self->windows[iWindow] == window) {
            self->iWindowFocused = iWindow;
        }
    }
    XSetInputFocus(self->display, window, RevertToPointerRoot, CurrentTime);
}

void Workspace_hideAllWindows(Workspace* self) {
    int monitorWidth = DisplayWidth(self->display, self->screen);
    for (unsigned long iWindow = 0; iWindow < self->nWindows; iWindow++) {
        XMoveWindow(self->display, self->windows[iWindow], -2*monitorWidth, 0);
    }
}

void Workspace_printWindowList(Workspace* self) {  /* DEBUG */
    for (unsigned long iWindow = 0; iWindow < self->nWindows; iWindow++) {
        printf("%lu ", self->windows[iWindow]);
    }
    printf("\n");
}

void Workspace_unHandleWindow(Workspace* self, Window window) {
    for (unsigned long iWindow = 0; iWindow < self->nWindows; iWindow++) {
        if (self->windows[iWindow] == window) {
            self->windows[iWindow] = 0;
            self->nWindows--;
            for (unsigned long iSubsequentWindow = iWindow; iSubsequentWindow < self->nWindows; iSubsequentWindow++) {
                self->windows[iSubsequentWindow] = self->windows[iSubsequentWindow + 1];
            }
            break;
        }
    }
}

void Workspace_tileWindows(Workspace* self) {
    int monitorWidth = (int)DisplayWidth(self->display, self->screen);
    int monitorHeight = (int)DisplayHeight(self->display, self->screen);

    if (self->nWindows == 1) {
        XMoveResizeWindow(self->display, self->windows[self->nWindows - 1], 0, 0, monitorWidth, monitorHeight);
    }
    else if (self->nWindows > 1) {
        int masterWindowWidth = (int)(monitorWidth * self->masterFactor);
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
