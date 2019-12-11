#include "gwm.h"

/* Constructor */
Task* Task_new(Display* display, const char* name) {
    Task* self = ecalloc(1, sizeof(*self));
    strcpy(self->name, name);
    for (int iWorkspace = 0; iWorkspace < NUMBER_OF_WORKSPACES; iWorkspace++) {
        self->workspaces[iWorkspace] = Workspace_new(display);
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
void Task_adjustMasterFactor(Task* self, double amount) {
    Workspace_adjustMasterFactor(self->workspaces[self->iWorkspaceActive], amount);
}

void Task_changeFocus(Task* self, int iOffset) {
    Workspace_changeFocus(self->workspaces[self->iWorkspaceActive], iOffset);
}

unsigned long Task_countWindows(Task* self) {
    unsigned long nWindows = 0;
    for (int iWorkspace = 0; iWorkspace < NUMBER_OF_WORKSPACES; iWorkspace++) {
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

void Task_printWindowList(Task* self) { /* DEBUG */
    for (int iWorkspace = 0; iWorkspace < NUMBER_OF_WORKSPACES; iWorkspace++) {
        printf("        Workspace %d: ", iWorkspace);
        Workspace_printWindowList(self->workspaces[iWorkspace]);
    }
}

void Task_switchWorkspace(Task* self, int iWorkspaceNew) {
    if (iWorkspaceNew == self->iWorkspaceActive) return;
    Task_hideAllWindows(self);
    self->iWorkspaceActive = iWorkspaceNew;
    Task_tileWindows(self);
}

void Task_tileWindows(Task* self) {
    Workspace_tileWindows(self->workspaces[self->iWorkspaceActive]);
}

void Task_unHandleWindow(Task* self, Window window) {
    for (int iWorkspace = 0; iWorkspace < NUMBER_OF_WORKSPACES; iWorkspace++) {
        Workspace_unHandleWindow(self->workspaces[iWorkspace], window);
    }
}
