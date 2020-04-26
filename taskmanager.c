#include "gwm.h"

/* Private function declarations */
static void TaskManager_regenerateTaskListString(TaskManager* self);

/* Constructor */
TaskManager* TaskManager_new(Display* display, int displayWidth, int displayHeight) {
    TaskManager* self = ecalloc(1, sizeof(*self));
    self->display = display;
    self->taskActive = Task_new(display, "default", displayWidth, displayHeight);
    self->nTasks = 1;
    TaskManager_regenerateTaskListString(self);
    return self;
}

/* Destructor */
TaskManager* TaskManager_free(TaskManager* self) {
    self->taskActive = Task_freeAll(self->taskActive);  /* frees all tasks recursively */
    free(self);
    return NULL;
}

/* Member functions */
void TaskManager_adjustMasterFactor(TaskManager* self, double amount) {
    Task_adjustMasterFactor(self->taskActive, amount);
}

void TaskManager_changeFocus(TaskManager* self, int iOffset) {
    Task_changeFocus(self->taskActive, iOffset);
}

void TaskManager_closeSelectedWindow(TaskManager* self) {
    Task_closeSelectedWindow(self->taskActive);
}

void TaskManager_focusWindow(TaskManager* self, Window window) {
    Task_focusWindow(self->taskActive, window);
}

void TaskManager_handleWindow(TaskManager* self, Window window) {
    Task_handleWindow(self->taskActive, window);
}

void TaskManager_makeSelectedWindowMaster(TaskManager* self) {
    Task_makeSelectedWindowMaster(self->taskActive);
}

void TaskManager_moveWindowToWorkspace(TaskManager* self, int iWorkspaceNew) {
    Task_moveWindowToWorkspace(self->taskActive, iWorkspaceNew);
}

void TaskManager_printWindowList(TaskManager* self) { /* DEBUG */
    puts("Window list:");
    for (Task* task = self->taskActive; task; task = task->taskNext) {
        printf("    %s\n", task->name);
        Task_printWindowList(task);
    }
}

void TaskManager_switchTask(TaskManager* self, const char* taskName, int displayWidth, int displayHeight) {
    Task* taskPrev = NULL;
    for (Task* task = self->taskActive; task; task = task->taskNext) {
        if (strcmp(task->name, taskName) == 0) {
            if (task == self->taskActive) return;
            printf("Changing task to already existing task '%s'...\n", taskName);
            Task_activate(task);
            Task_hideAllWindows(self->taskActive);

            taskPrev->taskNext = task->taskNext;
            if (Task_countWindows(self->taskActive) > 0) {
                task->taskNext = self->taskActive;
            }
            else {
                task->taskNext = self->taskActive->taskNext;
                self->taskActive = Task_free(self->taskActive);
            }
            self->taskActive = task;
            TaskManager_regenerateTaskListString(self);
            return;
        }
        taskPrev = task;
    }

    printf("Creating new task '%s'...\n", taskName);
    Task_hideAllWindows(self->taskActive);

    Task* taskNew = Task_new(self->display, taskName, displayWidth, displayHeight);
    if (Task_countWindows(self->taskActive) > 0) {
        taskNew->taskNext = self->taskActive;
    }
    else {
        taskNew->taskNext = self->taskActive->taskNext;
        self->taskActive = Task_free(self->taskActive);
    }
    self->taskActive = taskNew;

    Task_activate(self->taskActive);
    TaskManager_regenerateTaskListString(self);
}

void TaskManager_switchWorkspace(TaskManager* self, int iWorkspaceNew) {
    Task_switchWorkspace(self->taskActive, iWorkspaceNew);
}

void TaskManager_unHandleWindow(TaskManager* self, Window window) {
    for (Task* task = self->taskActive; task; task = task->taskNext) {
        Task_unHandleWindow(task, window);
    }
    Task_activate(self->taskActive);
}

void TaskManager_updateScreenResolution(TaskManager* self, int widthNew, int heightNew) {
    for (Task* task = self->taskActive; task; task = task->taskNext) {
        Task_updateScreenResolution(task, widthNew, heightNew);
    }
    Task_tileWindows(self->taskActive);
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
