#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

/* Type declarations */
typedef struct WindowManager WindowManager;
typedef struct Workspace Workspace;

/* User configuration */
#include "config.h"

/* Type definitions */
struct WindowManager {
    Display* display;
    Bool running;
    Workspace* workspaces[NUMBER_OF_WORKSPACES];
    int iWorkspaceActive;
};

struct Workspace {
    Display* display;
    int screen;
    Window windows[MAX_WINDOWS_PER_WORKSPACE];
    unsigned long nWindows;
};

/* Function declarations */
WindowManager* WindowManager_new(Display* display);
WindowManager* WindowManager_free(WindowManager* self);
void WindowManager_run(WindowManager* self);

Workspace* Workspace_new(Display* display);
Workspace* Workspace_free(Workspace* self);
void Workspace_handleWindow(Workspace* self, Window window);
void Workspace_focusWindow(Workspace* self, Window window);
void Workspace_hideAllWindows(Workspace* self);
void Workspace_printWindowList(Workspace* self);
void Workspace_unHandleWindow(Workspace* self, Window window);
void Workspace_tileWindows(Workspace* self);

/* util.c */
void* ecalloc(size_t nItems, size_t itemSize);
