/* Copyright (C) 2019-2020 Martin Gulliksson <martin@gullik.cc>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>


/* Set version string if not set by makefile */
#ifndef VERSION
#define VERSION "UNKNOWN_VERSION"
#endif


/* Type declarations */
typedef struct Task Task;


/* Function declarations */
static void* ecalloc(size_t nItems, size_t itemSize);
static int modulo(int a, int b);
static void spawn(const char* cmd[]);
static Task* taskCreate(const char* const name);
static void taskDestroyIfEmpty(Task* task, Task* taskPrev);
static void taskListStringRegenerate(void);
static void taskSwitch(const char* const name);
static void workspaceFocusChange(int offset);
static void workspaceHide(int iWorkspace);
static void workspaceMasterFactorAdjust(float amount);
static void workspaceShow(int iWorkspace);
static void workspaceSwitch(int iWorkspaceNew);
static void workspaceWindowAdd(Window window, int iWorkspace);
static void workspaceWindowClose(void);
static void workspaceWindowMakeMaster(void);
static void workspaceWindowMove(int iWorkspaceNew);
static void workspaceWindowRemove(Window window);
static int xErrorHandler(Display* display, XErrorEvent* event);
static void xEventConfigureNotify(XConfigureEvent* event);
static void xEventKeyPress(XKeyEvent* event);
static void xEventMapRequest(XMapRequestEvent* event);
static void xEventPropertyNotify(XPropertyEvent* event);
static void xEventUnmapNotify(XUnmapEvent* event);


/* User configuration */
#include "config.h"


/* Type definitions */
struct Task {
    char* name;
    Window windows[N_WORKSPACES_PER_TASK][MAX_WINDOWS_PER_WORKSPACE];
    int iWorkspaceActive;
    int nWindows[N_WORKSPACES_PER_TASK];
    int iWindowFocused[N_WORKSPACES_PER_TASK];
    float masterFactor[N_WORKSPACES_PER_TASK];
    Task* taskNext;
};


/* Global variables */
typedef struct Global {
    Display* display;
    int displayWidth;
    int displayHeight;
    bool running;
    Task* taskActive;
    char* taskListString;
    size_t taskListStringLength;
} Global;
Global gl;


/* Function definitions */
static void* ecalloc(size_t nItems, size_t itemSize) {
    void* pointer = calloc(nItems, itemSize);
    if (!pointer) {
        fputs("Error: failed to allocate memory\n", stderr);
        exit(EXIT_FAILURE);
    }
    return pointer;
}


static int modulo(int a, int b) {
    return (a % b + b) % b;
}


static void spawn(const char* cmd[]) {
    if (!fork()) {
        if (gl.display) {
            close(ConnectionNumber(gl.display));
        }

        int nSpecialTaskNames = sizeof(SPECIAL_TASK_NAMES) / sizeof(SPECIAL_TASK_NAMES[0]);
        for (int iSpecialTaskName = 0; iSpecialTaskName < nSpecialTaskNames; iSpecialTaskName++) {
            if (!strcmp(gl.taskActive->name, SPECIAL_TASK_NAMES[iSpecialTaskName])) {
                chdir(SPECIAL_TASK_WORKDIRS[iSpecialTaskName]);
                break;
            }
        }

        setsid();
        execvp(((char **)cmd)[0], (char **)cmd);
    }
}


static Task* taskCreate(const char* const name) {
    Task* task = ecalloc(1, sizeof(Task));

    size_t nameLength = strlen(name);
    task->name = ecalloc(nameLength + 1, sizeof(char));
    strcpy(task->name, name);

    for (int iWorkspace = 0; iWorkspace < N_WORKSPACES_PER_TASK; iWorkspace++) {
        task->masterFactor[iWorkspace] = MASTER_FACTOR_DEFAULT;
    }

    return task;
}


static void taskDestroyIfEmpty(Task* task, Task* taskPrev) {
    for (int iWorkspace = 0; iWorkspace < N_WORKSPACES_PER_TASK; iWorkspace++) {
        if (task->nWindows[iWorkspace]) return;
    }

    taskPrev->taskNext = task->taskNext;
    free(task->name);
    free(task);
}


static void taskListStringRegenerate(void) {
    size_t taskListStringLength = 1;
    for (Task* task = gl.taskActive; task; task = task->taskNext) {
        taskListStringLength += strlen(task->name) + 1;
    }

    if (taskListStringLength == gl.taskListStringLength) {
        memset(gl.taskListString, 0, gl.taskListStringLength * sizeof(char));
    }
    else {
        free(gl.taskListString);
        gl.taskListStringLength = taskListStringLength;
        gl.taskListString = ecalloc(gl.taskListStringLength, sizeof(char));
    }

    for (Task* task = gl.taskActive; task; task = task->taskNext) {
        strcat(gl.taskListString, task->name);
        strcat(gl.taskListString, "\n");
    }
}


static void taskSwitch(const char* const name) {
    if (!strcmp(gl.taskActive->name, name) || !strlen(name)) {
        return;
    }

    Task* task = NULL;
    Task* taskPrev = NULL;
    for (task = gl.taskActive; task; task = task->taskNext) {
        if (!strcmp(task->name, name)) {
            break;
        }
        taskPrev = task;
    }

    if (task) {
        taskPrev->taskNext = task->taskNext;
    }
    else {
        task = taskCreate(name);
    }

    task->taskNext = gl.taskActive;
    workspaceHide(gl.taskActive->iWorkspaceActive);
    gl.taskActive = task;
    workspaceShow(gl.taskActive->iWorkspaceActive);

    taskDestroyIfEmpty(gl.taskActive->taskNext, gl.taskActive);
    taskListStringRegenerate();
}


static void workspaceFocusChange(int offset) {
    int nWindows = gl.taskActive->nWindows[gl.taskActive->iWorkspaceActive];
    if (!nWindows) return;

    int iWorkspace = gl.taskActive->iWorkspaceActive;
    gl.taskActive->iWindowFocused[iWorkspace] = modulo(gl.taskActive->iWindowFocused[iWorkspace] + offset, nWindows);
    Window window = gl.taskActive->windows[iWorkspace][gl.taskActive->iWindowFocused[iWorkspace]];
    XSetInputFocus(gl.display, window, RevertToPointerRoot, CurrentTime);
}


static void workspaceHide(int iWorkspace) {
    for (int iWindow = 0; iWindow < gl.taskActive->nWindows[iWorkspace]; iWindow++) {
        Window window = gl.taskActive->windows[iWorkspace][iWindow];
        XMoveWindow(gl.display, window, 2 * gl.displayWidth, 2 * gl.displayHeight);
    }
}


static void workspaceMasterFactorAdjust(float amount) {
    int iWorkspace = gl.taskActive->iWorkspaceActive;
    gl.taskActive->masterFactor[iWorkspace] += amount;
    if (gl.taskActive->masterFactor[iWorkspace] > MASTER_FACTOR_MAX) gl.taskActive->masterFactor[iWorkspace] = MASTER_FACTOR_MAX;
    if (gl.taskActive->masterFactor[iWorkspace] < MASTER_FACTOR_MIN) gl.taskActive->masterFactor[iWorkspace] = MASTER_FACTOR_MIN;
    workspaceShow(iWorkspace);
}


static void workspaceShow(int iWorkspace) {
    int nWindows = gl.taskActive->nWindows[iWorkspace];
    if (!nWindows) {
        XSetInputFocus(gl.display, DefaultRootWindow(gl.display), RevertToPointerRoot, CurrentTime);
        return;
    }

    if (nWindows == 1) {
        XMoveResizeWindow(gl.display, gl.taskActive->windows[iWorkspace][0], 0, 0, gl.displayWidth, gl.displayHeight);
    }
    else {
        int masterWindowWidth = gl.displayWidth * gl.taskActive->masterFactor[iWorkspace];
        int stackWindowHeight = gl.displayHeight / (nWindows - 1);

        XMoveResizeWindow(gl.display, gl.taskActive->windows[iWorkspace][nWindows - 1], 0, 0, masterWindowWidth, gl.displayHeight);

        for (int iWindow = nWindows - 2; iWindow >= 0; iWindow--) {
            int x1 = masterWindowWidth;
            int y1 = stackWindowHeight * (nWindows - (iWindow + 2));
            int x2 = gl.displayWidth - x1;
            int y2 = stackWindowHeight;
            XMoveResizeWindow(gl.display, gl.taskActive->windows[iWorkspace][iWindow], x1, y1, x2, y2);
        }
    }

    if (gl.taskActive->iWindowFocused[iWorkspace] >= gl.taskActive->nWindows[iWorkspace]) {
        gl.taskActive->iWindowFocused[iWorkspace] = gl.taskActive->nWindows[iWorkspace] - 1;
    }
    Window windowFocused = gl.taskActive->windows[iWorkspace][gl.taskActive->iWindowFocused[iWorkspace]];
    XSetInputFocus(gl.display, windowFocused, RevertToPointerRoot, CurrentTime);
}


static void workspaceSwitch(int iWorkspaceNew) {
    if (iWorkspaceNew == gl.taskActive->iWorkspaceActive) return;
    workspaceShow(iWorkspaceNew);
    workspaceHide(gl.taskActive->iWorkspaceActive);
    gl.taskActive->iWorkspaceActive = iWorkspaceNew;
}


static void workspaceWindowAdd(Window window, int iWorkspace) {
    for (int iWindow = 0; iWindow < gl.taskActive->nWindows[iWorkspace]; iWindow++) {
        if (gl.taskActive->windows[iWorkspace][iWindow] == window) return;
    }

    gl.taskActive->windows[iWorkspace][gl.taskActive->nWindows[iWorkspace]] = window;
    gl.taskActive->nWindows[iWorkspace]++;

    XSelectInput(gl.display, window,
        EnterWindowMask|
        FocusChangeMask|
        KeyPressMask|
        StructureNotifyMask
    );
    XMapWindow(gl.display, window);

    gl.taskActive->iWindowFocused[iWorkspace] = gl.taskActive->nWindows[iWorkspace] - 1;

    if (iWorkspace == gl.taskActive->iWorkspaceActive) {
        Window windowFocused = gl.taskActive->windows[iWorkspace][gl.taskActive->iWindowFocused[iWorkspace]];
        XSetInputFocus(gl.display, windowFocused, RevertToPointerRoot, CurrentTime);
        workspaceShow(iWorkspace);
    }
}


static void workspaceWindowClose(void) {
    int nWindows = gl.taskActive->nWindows[gl.taskActive->iWorkspaceActive];
    if (!nWindows) return;

    int iWorkspace = gl.taskActive->iWorkspaceActive;
    int iWindowFocused = gl.taskActive->iWindowFocused[iWorkspace];
    Window windowFocused = gl.taskActive->windows[iWorkspace][iWindowFocused];

    XEvent event;
    event.xclient.type = ClientMessage;
    event.xclient.window = windowFocused;
    event.xclient.message_type = XInternAtom(gl.display, "WM_PROTOCOLS", true);
    event.xclient.format = 32;
    event.xclient.data.l[0] = XInternAtom(gl.display, "WM_DELETE_WINDOW", false);
    event.xclient.data.l[1] = CurrentTime;
    XSendEvent(gl.display, windowFocused, False, NoEventMask, &event);
}


static void workspaceWindowMakeMaster(void) {
    int nWindows = gl.taskActive->nWindows[gl.taskActive->iWorkspaceActive];
    if (nWindows < 2) return;

    int iWorkspace = gl.taskActive->iWorkspaceActive;
    int iWindowFocused = gl.taskActive->iWindowFocused[iWorkspace];

    Window window = (iWindowFocused == (nWindows - 1)) ? gl.taskActive->windows[iWorkspace][nWindows - 2] : gl.taskActive->windows[iWorkspace][iWindowFocused];
    workspaceWindowRemove(window);
    workspaceWindowAdd(window, gl.taskActive->iWorkspaceActive);
}


static void workspaceWindowMove(int iWorkspaceNew) {
    if (iWorkspaceNew == gl.taskActive->iWorkspaceActive) return;

    int nWindows = gl.taskActive->nWindows[gl.taskActive->iWorkspaceActive];
    if (!nWindows) return;

    int iWindowFocused = gl.taskActive->iWindowFocused[gl.taskActive->iWorkspaceActive];
    Window window = gl.taskActive->windows[gl.taskActive->iWorkspaceActive][iWindowFocused];
    workspaceWindowRemove(window);
    workspaceWindowAdd(window, iWorkspaceNew);
    workspaceHide(iWorkspaceNew);
    workspaceShow(gl.taskActive->iWorkspaceActive);
}


static void workspaceWindowRemove(Window window) {
    int iWorkspace = gl.taskActive->iWorkspaceActive;

    for (int iWindow = 0; iWindow < gl.taskActive->nWindows[iWorkspace]; iWindow++) {
        if (gl.taskActive->windows[iWorkspace][iWindow] == window) {
            gl.taskActive->windows[iWorkspace][iWindow] = 0;
            gl.taskActive->nWindows[iWorkspace]--;
            for (int iSubsequentWindow = iWindow; iSubsequentWindow < gl.taskActive->nWindows[iWorkspace]; iSubsequentWindow++) {
                gl.taskActive->windows[iWorkspace][iSubsequentWindow] = gl.taskActive->windows[iWorkspace][iSubsequentWindow + 1];
            }
            gl.taskActive->iWindowFocused[iWorkspace] = gl.taskActive->nWindows[iWorkspace] - 1;
            workspaceShow(iWorkspace);
            return;
        }
    }

    /* Window not in current workspace, check all other workspaces */
    for (Task* task = gl.taskActive; task; task = task->taskNext) {
        for (iWorkspace = 0; iWorkspace < N_WORKSPACES_PER_TASK; iWorkspace++) {
            for (int iWindow = 0; iWindow < task->nWindows[iWorkspace]; iWindow++) {
                if (task->windows[iWorkspace][iWindow] == window) {
                    task->windows[iWorkspace][iWindow] = 0;
                    task->nWindows[iWorkspace]--;
                    for (int iSubsequentWindow = iWindow; iSubsequentWindow < task->nWindows[iWorkspace]; iSubsequentWindow++) {
                        task->windows[iWorkspace][iSubsequentWindow] = task->windows[iWorkspace][iSubsequentWindow + 1];
                    }
                    gl.taskActive->iWindowFocused[iWorkspace] = gl.taskActive->nWindows[iWorkspace] - 1;
                    return;
                }
            }
        }
    }
}


static int xErrorHandler(Display* display, XErrorEvent* event) {
    enum {
        BUFFER_SIZE = 1024,
    };
    char errorText[BUFFER_SIZE];
    XGetErrorText(display, event->error_code, errorText, BUFFER_SIZE);
    fprintf(stderr, "Error occured: %s\n", errorText);
    fprintf(stderr, "    Display: %p\n", (void*)display);
    fprintf(stderr, "    XID resource id: %lu\n", event->resourceid);
    fprintf(stderr, "    Serial number of failed request: %lu\n", event->serial);
    fprintf(stderr, "    Error code of failed request: %u\n", (unsigned int)event->error_code);
    fprintf(stderr, "    Major op-code of failed request: %u\n", (unsigned int)event->request_code);
    fprintf(stderr, "    Minor op-code of failed request: %u\n", (unsigned int)event->minor_code);
    return 0;
}


static void xEventConfigureNotify(XConfigureEvent* event) {
    if (event->window == DefaultRootWindow(gl.display)) {
        gl.displayWidth = event->width;
        gl.displayHeight = event->height;
        workspaceShow(gl.taskActive->iWorkspaceActive);
    }
}


static void xEventKeyPress(XKeyEvent* event) {
    unsigned long keySym = XkbKeycodeToKeysym(gl.display, event->keycode, 0, 0);
    unsigned int modState = event->state;

    if (modState == keyMods[KEY_SWITCH_TO_WORKSPACE_0] && keySym == keys[KEY_SWITCH_TO_WORKSPACE_0]) {
        workspaceSwitch(0);
    }
    else if (modState == keyMods[KEY_SWITCH_TO_WORKSPACE_1] && keySym == keys[KEY_SWITCH_TO_WORKSPACE_1]) {
        workspaceSwitch(1);
    }
    else if (modState == keyMods[KEY_SWITCH_TO_WORKSPACE_2] && keySym == keys[KEY_SWITCH_TO_WORKSPACE_2]) {
        workspaceSwitch(2);
    }
    else if (modState == keyMods[KEY_SWITCH_TO_WORKSPACE_3] && keySym == keys[KEY_SWITCH_TO_WORKSPACE_3]) {
        workspaceSwitch(3);
    }
    else if (modState == keyMods[KEY_MOVE_WINDOW_TO_WORKSPACE_0] && keySym == keys[KEY_MOVE_WINDOW_TO_WORKSPACE_0]) {
        workspaceWindowMove(0);
    }
    else if (modState == keyMods[KEY_MOVE_WINDOW_TO_WORKSPACE_1] && keySym == keys[KEY_MOVE_WINDOW_TO_WORKSPACE_1]) {
        workspaceWindowMove(1);
    }
    else if (modState == keyMods[KEY_MOVE_WINDOW_TO_WORKSPACE_2] && keySym == keys[KEY_MOVE_WINDOW_TO_WORKSPACE_2]) {
        workspaceWindowMove(2);
    }
    else if (modState == keyMods[KEY_MOVE_WINDOW_TO_WORKSPACE_3] && keySym == keys[KEY_MOVE_WINDOW_TO_WORKSPACE_3]) {
        workspaceWindowMove(3);
    }
    else if (modState == keyMods[KEY_TERMINAL] && keySym == keys[KEY_TERMINAL]) {
        const char* cmd[]  = {TERMINAL, NULL};
        spawn(cmd);
    }
    else if (modState == keyMods[KEY_LAUNCHER] && keySym == keys[KEY_LAUNCHER]) {
        const char* cmd[]  = {LAUNCHER, NULL};
        spawn(cmd);
    }
    else if (modState == keyMods[KEY_TASK_SWITCHER] && keySym == keys[KEY_TASK_SWITCHER]) {
        const char* cmd[]  = {TASK_SWITCHER, gl.taskListString, NULL};
        spawn(cmd);
    }
    else if (modState == keyMods[KEY_MAKE_SELECTED_WINDOW_MASTER] && keySym == keys[KEY_MAKE_SELECTED_WINDOW_MASTER]) {
        workspaceWindowMakeMaster();
    }
    else if (modState == keyMods[KEY_MASTER_FACTOR_DEC] && keySym == keys[KEY_MASTER_FACTOR_DEC]) {
        workspaceMasterFactorAdjust(-MASTER_FACTOR_ADJUSTMENT_AMOUNT);
    }
    else if (modState == keyMods[KEY_MASTER_FACTOR_INC] && keySym == keys[KEY_MASTER_FACTOR_INC]) {
        workspaceMasterFactorAdjust(MASTER_FACTOR_ADJUSTMENT_AMOUNT);
    }
    else if (modState == keyMods[KEY_WINDOW_FOCUS_INC] && keySym == keys[KEY_WINDOW_FOCUS_INC]) {
        workspaceFocusChange(1);
    }
    else if (modState == keyMods[KEY_WINDOW_FOCUS_DEC] && keySym == keys[KEY_WINDOW_FOCUS_DEC]) {
        workspaceFocusChange(-1);
    }
    else if (modState == keyMods[KEY_WINDOW_CLOSE] && keySym == keys[KEY_WINDOW_CLOSE]) {
        workspaceWindowClose();
    }
    else if (modState == keyMods[KEY_GWM_EXIT] && keySym == keys[KEY_GWM_EXIT]) {
        gl.running = false;
    }
}


static void xEventMapRequest(XMapRequestEvent* event) {
    workspaceWindowAdd(event->window, gl.taskActive->iWorkspaceActive);
}


static void xEventPropertyNotify(XPropertyEvent* event) {
    if (event->window == DefaultRootWindow(gl.display) && event->atom == XA_WM_NAME) {
        XTextProperty xTextProperty;
        XGetTextProperty(gl.display, DefaultRootWindow(gl.display), &xTextProperty, event->atom);
        taskSwitch((char*)xTextProperty.value);
    }
}


static void xEventUnmapNotify(XUnmapEvent* event) {
    workspaceWindowRemove(event->window);
}


int main(void) {
    puts("This is gwm "VERSION);

    signal(SIGCHLD, SIG_IGN);
    memset(&gl, 0, sizeof(gl));

    gl.display = XOpenDisplay(NULL);
    if (!gl.display) {
        fputs("Error: failed to open display\n", stderr);
        exit(EXIT_FAILURE);
    }
    gl.displayWidth = DisplayWidth(gl.display, DefaultScreen(gl.display));
    gl.displayHeight = DisplayHeight(gl.display, DefaultScreen(gl.display));

    for (int iKey = 0; iKey < N_KEY_BINDINGS; iKey++) {
        XGrabKey(gl.display, XKeysymToKeycode(gl.display, keys[iKey]), keyMods[iKey],
                DefaultRootWindow(gl.display), true, GrabModeAsync, GrabModeAsync);
    }

    {
        XSetWindowAttributes wa;
        wa.cursor = None;
        wa.event_mask =
            SubstructureRedirectMask|
            ButtonPressMask|
            KeyPressMask|
            StructureNotifyMask|
            PropertyChangeMask;
        XChangeWindowAttributes(gl.display, DefaultRootWindow(gl.display), CWEventMask|CWCursor, &wa);
    }

    XSetErrorHandler(xErrorHandler);

    gl.taskActive = taskCreate(TASK_NAME_DEFAULT);
    taskListStringRegenerate();

    gl.running = true;

    {
        XEvent event;
        while (gl.running && !XNextEvent(gl.display, &event)) {
            switch (event.type) {
                case ConfigureNotify: xEventConfigureNotify(&event.xconfigure); break;
                case KeyPress: xEventKeyPress(&event.xkey); break;
                case MapRequest: xEventMapRequest(&event.xmaprequest); break;
                case PropertyNotify: xEventPropertyNotify(&event.xproperty); break;
                case UnmapNotify: xEventUnmapNotify(&event.xunmap); break;
            }
        }
    }

    free(gl.taskListString);

    {
        Task* task = gl.taskActive;
        while(task) {
            Task* taskTmp = task;
            task = task->taskNext;
            free(taskTmp->name);
            free(taskTmp);
        }
    }

    XCloseDisplay(gl.display);
}
