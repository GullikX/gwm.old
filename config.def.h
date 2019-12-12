#pragma once

#define MASTER_FACTOR 0.6
#define MASTER_FACTOR_MIN 0.1
#define MASTER_FACTOR_MAX 0.9
#define MAX_TASK_NAME_LENGTH 64
#define MAX_TASKS 64
#define MAX_WINDOWS_PER_WORKSPACE 256
#define NUMBER_OF_WORKSPACES 4
#define MODKEY Mod1Mask

static const char* SPECIAL_TASK_NAMES[] = {
    "tmp",
    "opt"
};

static const char* SPECIAL_TASK_WORKDIRS[] = {
    "/tmp",
    "/opt"
};
