#pragma once

enum {
    MAX_TASK_NAME_LENGTH = 64,
    MAX_TASKS = 64,
    MAX_WINDOWS_PER_WORKSPACE = 64,
    WORKSPACES_PER_TASK = 4,
    MODKEY = Mod4Mask,
};


static const double MASTER_FACTOR = 0.6;
static const double MASTER_FACTOR_MIN = 0.1;
static const double MASTER_FACTOR_MAX = 0.9;


static const char* TERMINAL = "st";
static const char* LAUNCHER = "dmenu_run";
static const char* TASK_SWITCHER = "gwm-taskswitcher";


static const char* SPECIAL_TASK_NAMES[] = {
    "tmp",
    "opt"
};
static const char* SPECIAL_TASK_WORKDIRS[] = {
    "/tmp",
    "/opt"
};
