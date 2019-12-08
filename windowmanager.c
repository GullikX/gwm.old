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
    self->workspace = Workspace_new(display);

    XGrabKey(self->display, XKeysymToKeycode(self->display, XStringToKeysym("XK_Return")), MODKEY,
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
    self->workspace = Workspace_free(self->workspace);
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
    Workspace_unHandleWindow(self->workspace, event->window);
    puts("destroyNotify end");
}

static void WindowManager_enterNotify(WindowManager* self, XCrossingEvent* event) {
    puts("enterNotify start");
    Workspace_focusWindow(self->workspace, event->window);
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
    puts("keyPress start");
    unsigned long keySym = XKeycodeToKeysym(self->display, event->keycode, 0);
    unsigned int modState = event->state;
    printf("got keysym %lu, modState %u\n", keySym, modState);
    puts("keyPress end");
}

static void WindowManager_mappingNotify(WindowManager* self, XMappingEvent* event) {
    (void)self; (void)event;
    puts("mappingNotify");
}

static void WindowManager_mapRequest(WindowManager* self, XMapRequestEvent* event) {
    puts("mapRequest start");
    Workspace_handleWindow(self->workspace, event->window);
    puts("mapRequest end");
}

static void WindowManager_motionNotify(WindowManager* self, XMotionEvent* event) {
    (void)self; (void)event;
    puts("motionNotify");
}

static void WindowManager_propertyNotify(WindowManager* self, XPropertyEvent* event) {
    (void)self; (void)event;
    puts("propertyNotify");
}

static void WindowManager_unmapNotify(WindowManager* self, XUnmapEvent* event) {
    (void)self; (void)event;
    puts("unmapNotify");
}
