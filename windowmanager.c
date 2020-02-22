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
    self->displayWidth = DisplayWidth(self->display, DefaultScreen(display));
    self->displayHeight = DisplayHeight(self->display, DefaultScreen(display));
    self->taskManager = TaskManager_new(display, self->displayWidth, self->displayHeight);

    XGrabKey(self->display, XKeysymToKeycode(self->display, XK_Return), MODKEY,
            DefaultRootWindow(self->display), True, GrabModeAsync, GrabModeAsync);

    XGrabKey(self->display, XKeysymToKeycode(self->display, XK_Return), MODKEY | ShiftMask,
            DefaultRootWindow(self->display), True, GrabModeAsync, GrabModeAsync);

    XGrabKey(self->display, XKeysymToKeycode(self->display, XK_Right), MODKEY,
            DefaultRootWindow(self->display), True, GrabModeAsync, GrabModeAsync);

    XGrabKey(self->display, XKeysymToKeycode(self->display, XK_Left), MODKEY,
            DefaultRootWindow(self->display), True, GrabModeAsync, GrabModeAsync);

    XGrabKey(self->display, XKeysymToKeycode(self->display, XK_Right), MODKEY | ShiftMask,
            DefaultRootWindow(self->display), True, GrabModeAsync, GrabModeAsync);

    XGrabKey(self->display, XKeysymToKeycode(self->display, XK_Left), MODKEY | ShiftMask,
            DefaultRootWindow(self->display), True, GrabModeAsync, GrabModeAsync);

    XGrabKey(self->display, XKeysymToKeycode(self->display, XK_space), MODKEY,
            DefaultRootWindow(self->display), True, GrabModeAsync, GrabModeAsync);

    XGrabKey(self->display, XKeysymToKeycode(self->display, XK_Tab), MODKEY,
            DefaultRootWindow(self->display), True, GrabModeAsync, GrabModeAsync);

    XGrabKey(self->display, XKeysymToKeycode(self->display, XK_a), MODKEY,
            DefaultRootWindow(self->display), True, GrabModeAsync, GrabModeAsync);

    XGrabKey(self->display, XKeysymToKeycode(self->display, XK_Escape), MODKEY | ShiftMask,
            DefaultRootWindow(self->display), True, GrabModeAsync, GrabModeAsync);

    XGrabKey(self->display, XKeysymToKeycode(self->display, XK_F4), MODKEY | ShiftMask,
            DefaultRootWindow(self->display), True, GrabModeAsync, GrabModeAsync);

    XGrabKey(self->display, XKeysymToKeycode(self->display, XK_1), MODKEY,
            DefaultRootWindow(self->display), True, GrabModeAsync, GrabModeAsync);

    XGrabKey(self->display, XKeysymToKeycode(self->display, XK_2), MODKEY,
            DefaultRootWindow(self->display), True, GrabModeAsync, GrabModeAsync);

    XGrabKey(self->display, XKeysymToKeycode(self->display, XK_3), MODKEY,
            DefaultRootWindow(self->display), True, GrabModeAsync, GrabModeAsync);

    XGrabKey(self->display, XKeysymToKeycode(self->display, XK_4), MODKEY,
            DefaultRootWindow(self->display), True, GrabModeAsync, GrabModeAsync);

    XGrabKey(self->display, XKeysymToKeycode(self->display, XK_1), MODKEY | ShiftMask,
            DefaultRootWindow(self->display), True, GrabModeAsync, GrabModeAsync);

    XGrabKey(self->display, XKeysymToKeycode(self->display, XK_2), MODKEY | ShiftMask,
            DefaultRootWindow(self->display), True, GrabModeAsync, GrabModeAsync);

    XGrabKey(self->display, XKeysymToKeycode(self->display, XK_3), MODKEY | ShiftMask,
            DefaultRootWindow(self->display), True, GrabModeAsync, GrabModeAsync);

    XGrabKey(self->display, XKeysymToKeycode(self->display, XK_4), MODKEY | ShiftMask,
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
    XSetErrorHandler(xErrorHandler);
    return self;
}

/* Destructor */
WindowManager* WindowManager_free(WindowManager* self) {
    self->taskManager = TaskManager_free(self->taskManager);
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
    puts("configureNotify start");
    if (event->window == DefaultRootWindow(self->display)) {
        self->displayWidth = event->width;
        self->displayHeight = event->height;
        TaskManager_updateScreenResolution(self->taskManager, event->width, event->height);
    }
    puts("configureNotify end");
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
    (void)self; (void)event;
    puts("destroyNotify");
}

static void WindowManager_enterNotify(WindowManager* self, XCrossingEvent* event) {
    (void)self; (void)event;
    puts("enterNotify start");
    //TaskManager_focusWindow(self->taskManager, event->window);
    puts("enterNotify end");
}

static void WindowManager_expose(WindowManager* self, XExposeEvent* event) {
    (void)self; (void)event;
    puts("expose");
}

static void WindowManager_focusIn(WindowManager* self, XFocusChangeEvent* event) {
    (void)self; (void)event;
    printf("focusIn, window %lu\n", event->window);
}

static void WindowManager_keyPress(WindowManager* self, XKeyEvent* event) {
    //puts("keyPress start");
    unsigned long keySym = XkbKeycodeToKeysym(self->display, event->keycode, 0, 0);
    unsigned int modState = event->state;
    //printf("got keysym %lu, modState %u\n", keySym, modState);

    if (modState == (MODKEY | ShiftMask) && keySym == XK_Escape) {
        puts("Exiting...");
        self->running = False;
    }
    else if (modState == MODKEY && keySym == XK_a) {
        TaskManager_printWindowList(self->taskManager);
    }
    else if (modState == MODKEY && keySym == XK_1) {
        TaskManager_switchWorkspace(self->taskManager, 0);
    }
    else if (modState == MODKEY && keySym == XK_2) {
        TaskManager_switchWorkspace(self->taskManager, 1);
    }
    else if (modState == MODKEY && keySym == XK_3) {
        TaskManager_switchWorkspace(self->taskManager, 2);
    }
    else if (modState == MODKEY && keySym == XK_4) {
        TaskManager_switchWorkspace(self->taskManager, 3);
    }
    else if (modState == (MODKEY | ShiftMask) && keySym == XK_1) {
        TaskManager_moveWindowToWorkspace(self->taskManager, 0);
    }
    else if (modState == (MODKEY | ShiftMask) && keySym == XK_2) {
        TaskManager_moveWindowToWorkspace(self->taskManager, 1);
    }
    else if (modState == (MODKEY | ShiftMask) && keySym == XK_3) {
        TaskManager_moveWindowToWorkspace(self->taskManager, 2);
    }
    else if (modState == (MODKEY | ShiftMask) && keySym == XK_4) {
        TaskManager_moveWindowToWorkspace(self->taskManager, 3);
    }
    else if (modState == MODKEY && keySym == XK_Return) {
        const char* cmd[]  = {TERMINAL, NULL};
        spawn(cmd, self->display, self->taskManager->taskActive->name);
    }
    else if (modState == (MODKEY | ShiftMask) && keySym == XK_Return) {
        const char* cmd[]  = {LAUNCHER, NULL};
        spawn(cmd, self->display, self->taskManager->taskActive->name);
    }
    else if (modState == MODKEY && keySym == XK_space) {
        const char* cmd[]  = {TASK_SWITCHER, self->taskManager->taskListString, NULL};
        spawn(cmd, self->display, self->taskManager->taskActive->name);
    }
    else if (modState == MODKEY && keySym == XK_Tab) {
        TaskManager_makeSelectedWindowMaster(self->taskManager);
    }
    else if (modState == (MODKEY | ShiftMask) && keySym == XK_Left) {
        TaskManager_adjustMasterFactor(self->taskManager, -0.05);
    }
    else if (modState == (MODKEY | ShiftMask) && keySym == XK_Right) {
        TaskManager_adjustMasterFactor(self->taskManager, 0.05);
    }
    else if (modState == MODKEY && keySym == XK_Left) {
        TaskManager_changeFocus(self->taskManager, 1);
    }
    else if (modState == MODKEY && keySym == XK_Right) {
        TaskManager_changeFocus(self->taskManager, -1);
    }
    else if (modState == MODKEY && keySym == XK_F4) {
        TaskManager_closeSelectedWindow(self->taskManager);
    }

    //puts("keyPress end");
}

static void WindowManager_mappingNotify(WindowManager* self, XMappingEvent* event) {
    (void)self; (void)event;
    puts("mappingNotify");
}

static void WindowManager_mapRequest(WindowManager* self, XMapRequestEvent* event) {
    puts("mapRequest start");
    TaskManager_handleWindow(self->taskManager, event->window);
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
		strncpy(rootWindowName, (char*)xTextProperty.value, MAX_TASK_NAME_LENGTH - 1);
        TaskManager_switchTask(self->taskManager, rootWindowName, self->displayWidth, self->displayHeight);
    }
}

static void WindowManager_unmapNotify(WindowManager* self, XUnmapEvent* event) {
    (void)self; (void)event;
    puts("unmapNotify start");
    TaskManager_unHandleWindow(self->taskManager, event->window);
    puts("unmapNotify end");
}
