#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>

/* Type declarations */
typedef struct EventManager EventManager;
typedef struct WindowManager WindowManager;

/* Type definitions */
struct EventManager {
    Display* display;
    Bool running;
    WindowManager* windowManager;
};

struct WindowManager {
    Display* display;
    int screen;
    Window windows[256];
    unsigned long nWindows;
};

/* Function declarations */
EventManager* EventManager_new(Display* display);
EventManager* EventManager_free(EventManager* self);
void EventManager_run(EventManager* self);

WindowManager* WindowManager_new(Display* display);
WindowManager* WindowManager_free(WindowManager* self);
void WindowManager_handleWindow(WindowManager* self, Window window);
void WindowManager_unHandleWindow(WindowManager* self, Window window);

/* util.c */
void* ecalloc(size_t nItems, size_t itemSize);

/* User configuration */
#include "config.h"
