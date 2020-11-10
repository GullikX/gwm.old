/* Config */
enum {
    MAX_WINDOWS_PER_WORKSPACE = 1024,
    N_WORKSPACES_PER_TASK = 4,
    MODKEY = Mod4Mask,
};

static const float MASTER_FACTOR_DEFAULT = 0.6f;
static const float MASTER_FACTOR_ADJUSTMENT_AMOUNT = 0.05f;
static const float MASTER_FACTOR_MIN = 0.1f;
static const float MASTER_FACTOR_MAX = 0.9f;

static const char* const TASK_NAME_DEFAULT = "default";

static const char* const TERMINAL = "st";
static const char* const LAUNCHER = "dmenu_run";
static const char* const TASK_SWITCHER = "gwm-taskswitcher";


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

    N_KEY_BINDINGS,
};

static const size_t keys[N_KEY_BINDINGS] = {
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
};

static const size_t keyMods[N_KEY_BINDINGS] = {
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
};


/* Task names recognized by gwm */
static const char* RECOGNIZED_TASK_NAMES[] = {
    "tmp",
    "opt"
};

static const char* RECOGNIZED_TASK_WORKDIRS[] = {
    "/tmp",
    "/opt"
};
