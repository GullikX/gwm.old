#include "gwm.h"

/* Private function declarations */
static void EventManager_buttonPress(EventManager* self, XButtonPressedEvent* event);
static void EventManager_clientMessage(EventManager* self, XClientMessageEvent* event);
static void EventManager_configureNotify(EventManager* self, XConfigureEvent* event);
static void EventManager_configureRequest(EventManager* self, XConfigureRequestEvent* event);
static void EventManager_createNotify(EventManager* self, XCreateWindowEvent* event);
static void EventManager_destroyNotify(EventManager* self, XDestroyWindowEvent* event);
static void EventManager_enterNotify(EventManager* self, XCrossingEvent* event);
static void EventManager_expose(EventManager* self, XExposeEvent* event);
static void EventManager_focusIn(EventManager* self, XFocusChangeEvent* event);
static void EventManager_keyPress(EventManager* self, XKeyEvent* event);
static void EventManager_mappingNotify(EventManager* self, XMappingEvent* event);
static void EventManager_mapRequest(EventManager* self, XMapRequestEvent* event);
static void EventManager_motionNotify(EventManager* self, XMotionEvent* event);
static void EventManager_propertyNotify(EventManager* self, XPropertyEvent* event);
static void EventManager_unmapNotify(EventManager* self, XUnmapEvent* event);

/* Constructor */
EventManager* EventManager_new(Display* display) {
    EventManager* self = ecalloc(1, sizeof(*self));
    self->display = display;
    self->running = True;
    XGrabKey(self->display, XKeysymToKeycode(self->display, XStringToKeysym("XK_Return")), MODKEY,
            DefaultRootWindow(self->display), True, GrabModeAsync, GrabModeAsync);

    XSetWindowAttributes wa;
    wa.cursor = None;
    wa.event_mask =
        SubstructureRedirectMask|
        SubstructureNotifyMask|
        ButtonPressMask|
        KeyPressMask|
        StructureNotifyMask|
        PropertyChangeMask;
    XChangeWindowAttributes(self->display, DefaultRootWindow(self->display), CWEventMask|CWCursor, &wa);
    return self;
}

/* Destructor */
EventManager* EventManager_free(EventManager* self) {
    free(self);
    return NULL;
}

/* Member functions */
void EventManager_run(EventManager* self) {
    XEvent event;
    while (self->running && !XNextEvent(self->display, &event)) {
        switch (event.type) {
            case ButtonPress: EventManager_buttonPress(self, &event.xbutton); break;
            case ClientMessage: EventManager_clientMessage(self, &event.xclient); break;
            case ConfigureNotify: EventManager_configureNotify(self, &event.xconfigure); break;
            case ConfigureRequest: EventManager_configureRequest(self, &event.xconfigurerequest); break;
            case CreateNotify: EventManager_createNotify(self, &event.xcreatewindow); break;
            case DestroyNotify: EventManager_destroyNotify(self, &event.xdestroywindow); break;
            case EnterNotify: EventManager_enterNotify(self, &event.xcrossing); break;
            case Expose: EventManager_expose(self, &event.xexpose); break;
            case FocusIn: EventManager_focusIn(self, &event.xfocus); break;
            case KeyPress: EventManager_keyPress(self, &event.xkey); break;
            case MappingNotify: EventManager_mappingNotify(self, &event.xmapping); break;
            case MapRequest: EventManager_mapRequest(self, &event.xmaprequest); break;
            case MotionNotify: EventManager_motionNotify(self, &event.xmotion); break;
            case PropertyNotify: EventManager_propertyNotify(self, &event.xproperty); break;
            case UnmapNotify: EventManager_unmapNotify(self, &event.xunmap); break;
            default: printf("Unknown event type %d\n", event.type);
        }
    }
}

/* Private member functions */
static void EventManager_buttonPress(EventManager* self, XButtonPressedEvent* event) {
    puts("ButtonPress");
}

static void EventManager_clientMessage(EventManager* self, XClientMessageEvent* event) {
    puts("ClientMessage");
}

static void EventManager_configureNotify(EventManager* self, XConfigureEvent* event) {
    puts("configureNotify");
}

static void EventManager_configureRequest(EventManager* self, XConfigureRequestEvent* event) {
    puts("configureRequest");
}

static void EventManager_createNotify(EventManager* self, XCreateWindowEvent* event) {
    puts("createNotify");
}

static void EventManager_destroyNotify(EventManager* self, XDestroyWindowEvent* event) {
    puts("destroyNotify");
}

static void EventManager_enterNotify(EventManager* self, XCrossingEvent* event) {
    puts("enterNotify");
}

static void EventManager_expose(EventManager* self, XExposeEvent* event) {
    puts("expose");
}

static void EventManager_focusIn(EventManager* self, XFocusChangeEvent* event) {
    puts("focusIn");
}

static void EventManager_keyPress(EventManager* self, XKeyEvent* event) {
    puts("keyPress");
}

static void EventManager_mappingNotify(EventManager* self, XMappingEvent* event) {
    puts("mappingNotify");
}

static void EventManager_mapRequest(EventManager* self, XMapRequestEvent* event) {
    puts("mapRequest");
}

static void EventManager_motionNotify(EventManager* self, XMotionEvent* event) {
    puts("motionNotify");
}

static void EventManager_propertyNotify(EventManager* self, XPropertyEvent* event) {
    puts("propertyNotify");
}

static void EventManager_unmapNotify(EventManager* self, XUnmapEvent* event) {
    puts("unmapNotify");
}
