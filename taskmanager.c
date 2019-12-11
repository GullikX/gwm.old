#include "gwm.h"

/* Constructor */
TaskManager* TaskManager_new(Display* display) {
    TaskManager* self = ecalloc(1, sizeof(*self));
    self->taskActive = Task_new(display, "default");
    self->nTasks = 1;
    return self;
}

/* Destructor */
TaskManager* TaskManager_free(TaskManager* self) {
    free(self->taskActive);  // TODO: free all tasks, not just first
    free(self);
    return NULL;
}

/* Member functions */
void TaskManager_focusWindow(TaskManager* self, Window window) {
    Task_focusWindow(self->taskActive, window);
}

void TaskManager_handleWindow(TaskManager* self, Window window) {
    Task_handleWindow(self->taskActive, window);
}

void TaskManager_switchWorkspace(TaskManager* self, int iWorkspaceNew) {
    Task_switchWorkspace(self->taskActive, iWorkspaceNew);
}

void TaskManager_unHandleWindow(TaskManager* self, Window window) {
    Task_unHandleWindow(self->taskActive, window);
}
