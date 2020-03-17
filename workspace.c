#include "gwm.h"

/* Constructor */
Workspace* Workspace_new(Display* display, int displayWidth, int displayHeight) {
    Workspace* self = ecalloc(1, sizeof(*self));
    self->display = display;
    self->nWindows = 0;
    self->masterFactor = MASTER_FACTOR;
    for (int iWindow = 0; iWindow < MAX_WINDOWS_PER_WORKSPACE; iWindow++) {
        self->windows[iWindow] = 0;
    }
    self->displayWidth = displayWidth;
    self->displayHeight = displayHeight;
    return self;
}

/* Destructor */
Workspace* Workspace_free(Workspace* self) {
    free(self);
    return NULL;
}

/* Member functions */
void Workspace_activate(Workspace* self) {
    Workspace_tileWindows(self);
    if (self->iWindowFocused < self->nWindows) {
        Workspace_focusWindow(self, self->windows[self->iWindowFocused]);
    }
}

void Workspace_adjustMasterFactor(Workspace* self, double amount) {
    self->masterFactor += amount;
    if (self->masterFactor > MASTER_FACTOR_MAX) self->masterFactor = MASTER_FACTOR_MAX;
    if (self->masterFactor < MASTER_FACTOR_MIN) self->masterFactor = MASTER_FACTOR_MIN;
    Workspace_tileWindows(self);
}

void Workspace_changeFocus(Workspace* self, int iOffset) {
    if (self->nWindows == 0) return;
    int iWindow = modulo(self->iWindowFocused + iOffset, self->nWindows);
    Workspace_focusWindow(self, self->windows[iWindow]);
}

void Workspace_closeSelectedWindow(Workspace* self) {
    if (self->nWindows == 0) return;
    XEvent event;
    event.xclient.type = ClientMessage;
    event.xclient.window = self->windows[self->iWindowFocused];
    event.xclient.message_type = XInternAtom(self->display, "WM_PROTOCOLS", True);
    event.xclient.format = 32;
    event.xclient.data.l[0] = XInternAtom(self->display, "WM_DELETE_WINDOW", False);
    event.xclient.data.l[1] = CurrentTime;
    XSendEvent(self->display, self->windows[self->iWindowFocused], False, NoEventMask, &event);
}

void Workspace_focusWindow(Workspace* self, Window window) {
    XSetInputFocus(self->display, window, RevertToPointerRoot, CurrentTime);
    for (int iWindow = 0; iWindow < self->nWindows; iWindow++) {
        if (self->windows[iWindow] == window) {
            self->iWindowFocused = iWindow;
            break;
        }
    }
}

void Workspace_handleWindow(Workspace* self, Window window) {
    for (int iWindow = 0; iWindow < self->nWindows; iWindow++) {
        if (self->windows[iWindow] == window) return;
    }

    self->windows[self->nWindows] = window;
    self->nWindows++;

    XMoveResizeWindow(self->display, window, 0, 0, self->displayWidth, self->displayHeight);
    XSelectInput(self->display, window,
        EnterWindowMask|
        FocusChangeMask|
        KeyPressMask|
        StructureNotifyMask
    );
    XMapWindow(self->display, window);
    Workspace_tileWindows(self);
    Workspace_focusWindow(self, window);
}

void Workspace_hideAllWindows(Workspace* self) {
    for (int iWindow = 0; iWindow < self->nWindows; iWindow++) {
        XMoveWindow(self->display, self->windows[iWindow], self->displayWidth * (-2), 0);
    }
}

void Workspace_makeSelectedWindowMaster(Workspace* self) {
    if (self->nWindows < 2) return;
    Window window = (self->iWindowFocused == (self->nWindows - 1)) ? self->windows[self->nWindows - 2] : self->windows[self->iWindowFocused];
    Workspace_unHandleWindow(self, window);
    Workspace_handleWindow(self, window);
}

void Workspace_printWindowList(Workspace* self) {  /* DEBUG */
    for (int iWindow = 0; iWindow < self->nWindows; iWindow++) {
        printf("%lu ", self->windows[iWindow]);
    }
    printf("\n");
}

void Workspace_unHandleWindow(Workspace* self, Window window) {
    for (int iWindow = 0; iWindow < self->nWindows; iWindow++) {
        if (self->windows[iWindow] == window) {
            self->windows[iWindow] = 0;
            self->nWindows--;
            for (int iSubsequentWindow = iWindow; iSubsequentWindow < self->nWindows; iSubsequentWindow++) {
                self->windows[iSubsequentWindow] = self->windows[iSubsequentWindow + 1];
            }
            if (self->nWindows > 0) {
                Workspace_focusWindow(self, self->windows[self->nWindows - 1]);
            }
            break;
        }
    }
}

void Workspace_tileWindows(Workspace* self) {
    if (self->nWindows == 1) {
        XMoveResizeWindow(self->display, self->windows[self->nWindows - 1], 0, 0, self->displayWidth, self->displayHeight);
    }
    else if (self->nWindows > 1) {
        int masterWindowWidth = self->displayWidth * self->masterFactor;
        int stackWindowHeight = self->displayHeight / (self->nWindows - 1);

        XMoveResizeWindow(self->display, self->windows[self->nWindows - 1], 0, 0, masterWindowWidth, self->displayHeight);

        for (int iWindow = self->nWindows - 2; iWindow >= 0; iWindow--) {
            int x1 = masterWindowWidth;
            int y1 = stackWindowHeight * (self->nWindows - (iWindow + 2));
            int x2 = self->displayWidth - x1;
            int y2 = stackWindowHeight;
            XMoveResizeWindow(self->display, self->windows[iWindow], x1, y1, x2, y2);
        }
    }
}

void Workspace_updateScreenResolution(Workspace* self, int widthNew, int heightNew) {
    self->displayWidth = widthNew;
    self->displayHeight = heightNew;
}
