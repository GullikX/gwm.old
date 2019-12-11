#include "gwm.h"

/* Private function declarations */
static void TaskManager_regenerateTaskListString(TaskManager* self);

/* Constructor */
TaskManager* TaskManager_new(Display* display) {
    TaskManager* self = ecalloc(1, sizeof(*self));
    self->taskActive = Task_new(display, "default");
    self->nTasks = 1;
    TaskManager_regenerateTaskListString(self);
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

void TaskManager_printWindowList(TaskManager* self) { /* DEBUG */
    puts("Window list:");
    for (Task* task = self->taskActive; task; task = task->taskNext) {
        printf("    %s\n", task->name);
        Task_printWindowList(task);
    }
}

void TaskManager_switchWorkspace(TaskManager* self, int iWorkspaceNew) {
    Task_switchWorkspace(self->taskActive, iWorkspaceNew);
}

void TaskManager_unHandleWindow(TaskManager* self, Window window) {
    Task_unHandleWindow(self->taskActive, window);
}

/* Private member functions */
static void TaskManager_regenerateTaskListString(TaskManager* self) {
    int iChar = 0;
    for (Task* task = self->taskActive; task; task = task->taskNext) {
        for (unsigned long jChar = 0; jChar < strlen(task->name); jChar++) {
            self->taskListString[iChar] = task->name[jChar];
            iChar++;
        }
        self->taskListString[iChar] = '\n';
        iChar++;
    }
    self->taskListString[iChar] = '\0';
}
