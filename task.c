#include "gwm.h"

/* Constructor */
Task* Task_new(Display* display, const char* name, int displayWidth, int displayHeight) {
    Task* self = ecalloc(1, sizeof(*self));
    strcpy(self->name, name);
    for (int iWorkspace = 0; iWorkspace < WORKSPACES_PER_TASK; iWorkspace++) {
        self->workspaces[iWorkspace] = Workspace_new(display, displayWidth, displayHeight);
    }
    self->iWorkspaceActive = 0;
    self->taskNext = NULL;
    return self;
}

/* Destructor */
Task* Task_free(Task* self) {
    free(self);
    return NULL;
}

/* Member functions */
void Task_activate(Task* self) {
    Workspace_activate(self->workspaces[self->iWorkspaceActive]);
}

void Task_adjustMasterFactor(Task* self, double amount) {
    Workspace_adjustMasterFactor(self->workspaces[self->iWorkspaceActive], amount);
}

void Task_changeFocus(Task* self, int iOffset) {
    Workspace_changeFocus(self->workspaces[self->iWorkspaceActive], iOffset);
}

unsigned long Task_countWindows(Task* self) {
    unsigned long nWindows = 0;
    for (int iWorkspace = 0; iWorkspace < WORKSPACES_PER_TASK; iWorkspace++) {
        nWindows += self->workspaces[iWorkspace]->nWindows;
    }
    return nWindows;
}

void Task_focusWindow(Task* self, Window window) {
    Workspace_focusWindow(self->workspaces[self->iWorkspaceActive], window);
}

void Task_handleWindow(Task* self, Window window) {
    Workspace_handleWindow(self->workspaces[self->iWorkspaceActive], window);
}

void Task_hideAllWindows(Task* self) {
    Workspace_hideAllWindows(self->workspaces[self->iWorkspaceActive]);
}

void Task_makeSelectedWindowMaster(Task* self) {
    Workspace_makeSelectedWindowMaster(self->workspaces[self->iWorkspaceActive]);
}

void Task_moveWindowToWorkspace(Task* self, int iWorkspaceNew) {
    if (iWorkspaceNew == self->iWorkspaceActive) return;
    Workspace* workspaceActive = self->workspaces[self->iWorkspaceActive];
    Window window = workspaceActive->windows[workspaceActive->iWindowFocused];
    Workspace_unHandleWindow(workspaceActive, window);
    Workspace_handleWindow(self->workspaces[iWorkspaceNew], window);
    Workspace_hideAllWindows(self->workspaces[iWorkspaceNew]);
    Workspace_tileWindows(workspaceActive);
}

void Task_printWindowList(Task* self) { /* DEBUG */
    for (int iWorkspace = 0; iWorkspace < WORKSPACES_PER_TASK; iWorkspace++) {
        printf("        Workspace %d: ", iWorkspace);
        Workspace_printWindowList(self->workspaces[iWorkspace]);
    }
}

void Task_switchWorkspace(Task* self, int iWorkspaceNew) {
    if (iWorkspaceNew == self->iWorkspaceActive) return;
    Workspace_activate(self->workspaces[iWorkspaceNew]);
    Workspace_hideAllWindows(self->workspaces[self->iWorkspaceActive]);
    self->iWorkspaceActive = iWorkspaceNew;
}

void Task_tileWindows(Task* self) {
    Workspace_tileWindows(self->workspaces[self->iWorkspaceActive]);
}

void Task_unHandleWindow(Task* self, Window window) {
    for (int iWorkspace = 0; iWorkspace < WORKSPACES_PER_TASK; iWorkspace++) {
        Workspace_unHandleWindow(self->workspaces[iWorkspace], window);
    }
}

void Task_updateScreenResolution(Task* self, int widthNew, int heightNew) {
    for (int iWorkspace = 0; iWorkspace < WORKSPACES_PER_TASK; iWorkspace++) {
        Workspace_updateScreenResolution(self->workspaces[iWorkspace], widthNew, heightNew);
    }
}
