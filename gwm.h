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
typedef struct Task Task;
typedef struct TaskManager TaskManager;
typedef struct WindowManager WindowManager;
typedef struct Workspace Workspace;

/* User configuration */
#include "config.h"

/* Type definitions */
struct Task {
    char name[MAX_TASK_NAME_LENGTH];
    Workspace* workspaces[NUMBER_OF_WORKSPACES];
    int iWorkspaceActive;
    Task* taskNext;
};

struct TaskManager {
    Task* taskActive;
    int nTasks;
    char taskList[MAX_TASK_NAME_LENGTH * MAX_TASKS];
};

struct WindowManager {
    Display* display;
    Bool running;
    TaskManager* taskManager;
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

TaskManager* TaskManager_new(Display* display);
TaskManager* TaskManager_free(TaskManager* self);
void Task_focusWindow(Task* self, Window window);
void Task_handleWindow(Task* self, Window window);
void Task_switchWorkspace(Task* self, int iWorkspaceNew);
void Task_unHandleWindow(Task* self, Window window);

Task* Task_new(Display* display, const char* name);
Task* Task_free(Task* self);
void TaskManager_focusWindow(TaskManager* self, Window window);
void TaskManager_handleWindow(TaskManager* self, Window window);
void TaskManager_switchWorkspace(TaskManager* self, int iWorkspaceNew);
void TaskManager_unHandleWindow(TaskManager* self, Window window);

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
void spawn(Display* display, const char* cmd[]);
