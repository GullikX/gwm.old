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

/* Macros */
#define LENGTH(A) (sizeof(A) / sizeof(A[0]))

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
    Display* display;
    Task* taskActive;
    int nTasks;
    char taskListString[MAX_TASK_NAME_LENGTH * MAX_TASKS];
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
    int iWindowFocused;
    double masterFactor;
};

/* Function declarations */
WindowManager* WindowManager_new(Display* display);
WindowManager* WindowManager_free(WindowManager* self);
void WindowManager_run(WindowManager* self);

Task* Task_new(Display* display, const char* name);
Task* Task_free(Task* self);
void Task_adjustMasterFactor(Task* self, double amount);
void Task_changeFocus(Task* self, int iOffset);
unsigned long Task_countWindows(Task* self);
void Task_focusWindow(Task* self, Window window);
void Task_handleWindow(Task* self, Window window);
void Task_hideAllWindows(Task* self);
void Task_moveWindowToWorkspace(Task* self, int iWorkspaceNew);
void Task_printWindowList(Task* self);
void Task_switchWorkspace(Task* self, int iWorkspaceNew);
void Task_tileWindows(Task* self);
void Task_unHandleWindow(Task* self, Window window);

TaskManager* TaskManager_new(Display* display);
TaskManager* TaskManager_free(TaskManager* self);
void TaskManager_adjustMasterFactor(TaskManager* self, double amount);
void TaskManager_changeFocus(TaskManager* self, int iOffset);
void TaskManager_focusWindow(TaskManager* self, Window window);
void TaskManager_handleWindow(TaskManager* self, Window window);
void TaskManager_moveWindowToWorkspace(TaskManager* self, int iWorkspaceNew);
void TaskManager_printWindowList(TaskManager* self);
void TaskManager_switchTask(TaskManager* self, const char* taskName);
void TaskManager_switchWorkspace(TaskManager* self, int iWorkspaceNew);
void TaskManager_unHandleWindow(TaskManager* self, Window window);

Workspace* Workspace_new(Display* display);
Workspace* Workspace_free(Workspace* self);
void Workspace_adjustMasterFactor(Workspace* self, double amount);
void Workspace_changeFocus(Workspace* self, int iOffset);
void Workspace_handleWindow(Workspace* self, Window window);
void Workspace_focusWindow(Workspace* self, Window window);
void Workspace_hideAllWindows(Workspace* self);
void Workspace_printWindowList(Workspace* self);
void Workspace_unHandleWindow(Workspace* self, Window window);
void Workspace_tileWindows(Workspace* self);

/* util.c */
void* ecalloc(size_t nItems, size_t itemSize);
int modulo(int a, int b);
void spawn(const char* cmd[], Display* display, const char* taskName);
