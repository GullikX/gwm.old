#include "gwm.h"

/* Private function declarations */
static void WindowManager_buttonPress(WindowManager* self, XButtonPressedEvent* event);
static void WindowManager_clientMessage(WindowManager* self, XClientMessageEvent* event);
static void WindowManager_configureNotify(WindowManager* self, XConfigureEvent* event);
static void WindowManager_configureRequest(WindowManager* self, XConfigureRequestEvent* event);
static void WindowManager_createNotify(WindowManager* self, XCreateWindowEvent* event);
static void WindowManager_destroyNotify(WindowManager* self, XDestroyWindowEvent* event);
static void WindowManager_enterNotify(WindowManager* self, XCrossingEvent* event);
static void WindowManager_expose(WindowManager* self, XExposeEvent* event);
static void WindowManager_focusIn(WindowManager* self, XFocusChangeEvent* event);
static void WindowManager_keyPress(WindowManager* self, XKeyEvent* event);
static void WindowManager_mappingNotify(WindowManager* self, XMappingEvent* event);
static void WindowManager_mapRequest(WindowManager* self, XMapRequestEvent* event);
static void WindowManager_motionNotify(WindowManager* self, XMotionEvent* event);
static void WindowManager_propertyNotify(WindowManager* self, XPropertyEvent* event);
static void WindowManager_unmapNotify(WindowManager* self, XUnmapEvent* event);

/* Constructor */
WindowManager* WindowManager_new(Display* display) {
    WindowManager* self = ecalloc(1, sizeof(*self));
    self->display = display;
    self->running = True;
    for (int iWorkspace = 0; iWorkspace < NUMBER_OF_WORKSPACES; iWorkspace++) {
        self->workspaces[iWorkspace] = Workspace_new(display);
    }
    self->iWorkspaceActive = 0;

    XGrabKey(self->display, XKeysymToKeycode(self->display, XK_Return), MODKEY,
            DefaultRootWindow(self->display), True, GrabModeAsync, GrabModeAsync);

    XGrabKey(self->display, XKeysymToKeycode(self->display, XK_t), MODKEY,
            DefaultRootWindow(self->display), True, GrabModeAsync, GrabModeAsync);

    XGrabKey(self->display, XKeysymToKeycode(self->display, XK_a), MODKEY,
            DefaultRootWindow(self->display), True, GrabModeAsync, GrabModeAsync);

    XGrabKey(self->display, XKeysymToKeycode(self->display, XK_q), MODKEY | ShiftMask,
            DefaultRootWindow(self->display), True, GrabModeAsync, GrabModeAsync);

    XGrabKey(self->display, XKeysymToKeycode(self->display, XK_1), MODKEY,
            DefaultRootWindow(self->display), True, GrabModeAsync, GrabModeAsync);

    XGrabKey(self->display, XKeysymToKeycode(self->display, XK_2), MODKEY,
            DefaultRootWindow(self->display), True, GrabModeAsync, GrabModeAsync);

    XGrabKey(self->display, XKeysymToKeycode(self->display, XK_3), MODKEY,
            DefaultRootWindow(self->display), True, GrabModeAsync, GrabModeAsync);

    XGrabKey(self->display, XKeysymToKeycode(self->display, XK_4), MODKEY,
            DefaultRootWindow(self->display), True, GrabModeAsync, GrabModeAsync);

    XSetWindowAttributes wa;
    wa.cursor = None;
    wa.event_mask =
        SubstructureRedirectMask|
        ButtonPressMask|
        KeyPressMask|
        StructureNotifyMask|
        PropertyChangeMask;
    XChangeWindowAttributes(self->display, DefaultRootWindow(self->display), CWEventMask|CWCursor, &wa);
    return self;
}

/* Destructor */
WindowManager* WindowManager_free(WindowManager* self) {
    for (int iWorkspace = 0; iWorkspace < NUMBER_OF_WORKSPACES; iWorkspace++) {
        self->workspaces[iWorkspace] = Workspace_free(self->workspaces[iWorkspace]);
    }
    free(self);
    return NULL;
}

/* Member functions */
void WindowManager_run(WindowManager* self) {
    XEvent event;
    while (self->running && !XNextEvent(self->display, &event)) {
        switch (event.type) {
            case ButtonPress: WindowManager_buttonPress(self, &event.xbutton); break;
            case ClientMessage: WindowManager_clientMessage(self, &event.xclient); break;
            case ConfigureNotify: WindowManager_configureNotify(self, &event.xconfigure); break;
            case ConfigureRequest: WindowManager_configureRequest(self, &event.xconfigurerequest); break;
            case CreateNotify: WindowManager_createNotify(self, &event.xcreatewindow); break;
            case DestroyNotify: WindowManager_destroyNotify(self, &event.xdestroywindow); break;
            case EnterNotify: WindowManager_enterNotify(self, &event.xcrossing); break;
            case Expose: WindowManager_expose(self, &event.xexpose); break;
            case FocusIn: WindowManager_focusIn(self, &event.xfocus); break;
            case KeyPress: WindowManager_keyPress(self, &event.xkey); break;
            case MappingNotify: WindowManager_mappingNotify(self, &event.xmapping); break;
            case MapRequest: WindowManager_mapRequest(self, &event.xmaprequest); break;
            case MotionNotify: WindowManager_motionNotify(self, &event.xmotion); break;
            case PropertyNotify: WindowManager_propertyNotify(self, &event.xproperty); break;
            case UnmapNotify: WindowManager_unmapNotify(self, &event.xunmap); break;
            default: printf("Unknown event type %d\n", event.type);
        }
    }
}

void WindowManager_switchWorkspace(WindowManager* self, int iWorkspaceNew) {
    if (iWorkspaceNew == self->iWorkspaceActive) return;
    Workspace_hideAllWindows(self->workspaces[self->iWorkspaceActive]);
    Workspace_tileWindows(self->workspaces[iWorkspaceNew]);
    self->iWorkspaceActive = iWorkspaceNew;
}

/* Private member functions */
static void WindowManager_buttonPress(WindowManager* self, XButtonPressedEvent* event) {
    (void)self; (void)event;
    puts("ButtonPress");
}

static void WindowManager_clientMessage(WindowManager* self, XClientMessageEvent* event) {
    (void)self; (void)event;
    puts("ClientMessage");
}

static void WindowManager_configureNotify(WindowManager* self, XConfigureEvent* event) {
    (void)self; (void)event;
    puts("configureNotify");
}

static void WindowManager_configureRequest(WindowManager* self, XConfigureRequestEvent* event) {
    (void)self; (void)event;
    puts("configureRequest");
}

static void WindowManager_createNotify(WindowManager* self, XCreateWindowEvent* event) {
    (void)self; (void)event;
    puts("createNotify");
}

static void WindowManager_destroyNotify(WindowManager* self, XDestroyWindowEvent* event) {
    puts("destroyNotify start");
    Workspace_unHandleWindow(self->workspaces[self->iWorkspaceActive], event->window);
    puts("destroyNotify end");
}

static void WindowManager_enterNotify(WindowManager* self, XCrossingEvent* event) {
    puts("enterNotify start");
    Workspace_focusWindow(self->workspaces[self->iWorkspaceActive], event->window);
    puts("enterNotify end");
}

static void WindowManager_expose(WindowManager* self, XExposeEvent* event) {
    (void)self; (void)event;
    puts("expose");
}

static void WindowManager_focusIn(WindowManager* self, XFocusChangeEvent* event) {
    (void)self; (void)event;
    puts("focusIn");
}

static void WindowManager_keyPress(WindowManager* self, XKeyEvent* event) {
    //puts("keyPress start");
    unsigned long keySym = XkbKeycodeToKeysym(self->display, event->keycode, 0, 0);
    unsigned int modState = event->state;
    //printf("got keysym %lu, modState %u\n", keySym, modState);

    if (modState == (MODKEY | ShiftMask) && keySym == XK_q) {
        puts("Exiting...");
        self->running = False;
    }
    else if (modState == MODKEY && keySym == XK_a) {
        for (int iWorkspace = 0; iWorkspace < NUMBER_OF_WORKSPACES; iWorkspace++) {
            printf("Workspace %d ", iWorkspace);
            Workspace_printWindowList(self->workspaces[iWorkspace]);
        }
    }
    else if(modState == MODKEY && keySym == XK_1) {
        WindowManager_switchWorkspace(self, 0);
    }
    else if (modState == MODKEY && keySym == XK_2) {
        WindowManager_switchWorkspace(self, 1);
    }
    else if (modState == MODKEY && keySym == XK_3) {
        WindowManager_switchWorkspace(self, 2);
    }
    else if (modState == MODKEY && keySym == XK_4) {
        WindowManager_switchWorkspace(self, 3);
    }
    else if (modState == MODKEY && keySym == XK_Return) {
        const char* cmd[]  = {"st", NULL};
        spawn(self->display, cmd);
    }
    else if (modState == MODKEY && keySym == XK_t) {
        const char* cmd[]  = {"gwm-taskswitcher", "default\nsurf\nmusic", NULL};
        spawn(self->display, cmd);
    }

    //puts("keyPress end");
}

static void WindowManager_mappingNotify(WindowManager* self, XMappingEvent* event) {
    (void)self; (void)event;
    puts("mappingNotify");
}

static void WindowManager_mapRequest(WindowManager* self, XMapRequestEvent* event) {
    puts("mapRequest start");
    Workspace_handleWindow(self->workspaces[self->iWorkspaceActive], event->window);
    puts("mapRequest end");
}

static void WindowManager_motionNotify(WindowManager* self, XMotionEvent* event) {
    (void)self; (void)event;
    puts("motionNotify");
}

static void WindowManager_propertyNotify(WindowManager* self, XPropertyEvent* event) {
    if (event->window == DefaultRootWindow(self->display) && event->atom == XA_WM_NAME) {
        XTextProperty xTextProperty;
        XGetTextProperty(self->display, DefaultRootWindow(self->display), &xTextProperty, event->atom);
        char rootWindowName[MAX_TASK_NAME_LENGTH];
		strncpy(rootWindowName, (char *)xTextProperty.value, MAX_TASK_NAME_LENGTH - 1);
        printf("Root window name updated to '%s'.\n", rootWindowName);
    }
}

static void WindowManager_unmapNotify(WindowManager* self, XUnmapEvent* event) {
    (void)self; (void)event;
    puts("unmapNotify");
}
