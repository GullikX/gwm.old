#pragma once

/* Config */
enum {
    MAX_TASK_NAME_LENGTH = 64,
    MAX_TASKS = 64,
    MAX_WINDOWS_PER_WORKSPACE = 64,
    WORKSPACES_PER_TASK = 4,
    MODKEY = Mod4Mask,
};

static const double MASTER_FACTOR = 0.6;
static const double MASTER_FACTOR_ADJUSTMENT_AMOUNT = 0.05;
static const double MASTER_FACTOR_MIN = 0.1;
static const double MASTER_FACTOR_MAX = 0.9;

static const char* TERMINAL = "st";
static const char* LAUNCHER = "dmenu_run";
static const char* TASK_SWITCHER = "gwm-taskswitcher";


/* Keybindings */
enum {
    KEY_SWITCH_TO_WORKSPACE_0,
    KEY_SWITCH_TO_WORKSPACE_1,
    KEY_SWITCH_TO_WORKSPACE_2,
    KEY_SWITCH_TO_WORKSPACE_3,

    KEY_MOVE_WINDOW_TO_WORKSPACE_0,
    KEY_MOVE_WINDOW_TO_WORKSPACE_1,
    KEY_MOVE_WINDOW_TO_WORKSPACE_2,
    KEY_MOVE_WINDOW_TO_WORKSPACE_3,

    KEY_TERMINAL,
    KEY_LAUNCHER,
    KEY_TASK_SWITCHER,

    KEY_MAKE_SELECTED_WINDOW_MASTER,
    KEY_MASTER_FACTOR_DEC,
    KEY_MASTER_FACTOR_INC,

    KEY_WINDOW_FOCUS_DEC,
    KEY_WINDOW_FOCUS_INC,
    KEY_WINDOW_CLOSE,

    KEY_GWM_EXIT,

    KEY_PRINT_WINDOW_LIST,  /* Debug */

    N_KEY_BINDINGS,
};

static const size_t keys[] = {
    XK_1,
    XK_2,
    XK_3,
    XK_4,

    XK_1,
    XK_2,
    XK_3,
    XK_4,

    XK_Return,
    XK_Return,
    XK_space,

    XK_Tab,
    XK_Left,
    XK_Right,

    XK_Right,
    XK_Left,
    XK_F4,

    XK_F12,

    XK_a,
};

static const size_t keyMods[] = {
    MODKEY,
    MODKEY,
    MODKEY,
    MODKEY,

    MODKEY | ShiftMask,
    MODKEY | ShiftMask,
    MODKEY | ShiftMask,
    MODKEY | ShiftMask,

    MODKEY,
    MODKEY | ShiftMask,
    MODKEY,

    MODKEY,
    MODKEY | ShiftMask,
    MODKEY | ShiftMask,

    MODKEY,
    MODKEY,
    MODKEY,

    MODKEY | ShiftMask,

    MODKEY,
};


/* Task names recognized by gwm */
static const char* SPECIAL_TASK_NAMES[] = {
    "tmp",
    "opt"
};

static const char* SPECIAL_TASK_WORKDIRS[] = {
    "/tmp",
    "/opt"
};
