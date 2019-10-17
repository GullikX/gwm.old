#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>

/* Type declarations */
typedef struct EventManager EventManager;

/* Type definitions */
struct EventManager {
    Display* display;
    int screen;
    bool running;
};

/* Function declarations */
EventManager* EventManager_new(Display* display);
EventManager* EventManager_free(EventManager* self);
void EventManager_run(EventManager* self);

/* util.c */
void* ecalloc(size_t nItems, size_t itemSize);

/* User configuration */
#include "config.h"
