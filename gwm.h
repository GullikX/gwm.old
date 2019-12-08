#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>

/* Type declarations */
typedef struct EventManager EventManager;
typedef struct Workspace Workspace;

/* User configuration */
#include "config.h"

/* Type definitions */
struct EventManager {
    Display* display;
    Bool running;
    Workspace* workspace;
};

struct Workspace {
    Display* display;
    int screen;
    Window windows[MAX_WINDOWS];
    unsigned long nWindows;
};

/* Function declarations */
EventManager* EventManager_new(Display* display);
EventManager* EventManager_free(EventManager* self);
void EventManager_run(EventManager* self);

Workspace* Workspace_new(Display* display);
Workspace* Workspace_free(Workspace* self);
void Workspace_handleWindow(Workspace* self, Window window);
void Workspace_focusWindow(Workspace* self, Window window);
void Workspace_unHandleWindow(Workspace* self, Window window);

/* util.c */
void* ecalloc(size_t nItems, size_t itemSize);
