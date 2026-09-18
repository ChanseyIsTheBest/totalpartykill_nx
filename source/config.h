/* config.h -- settings.
 *
 * Everything about this port is fixed at build time except one thing: which
 * Switch button presses which of the game's on-screen controls. That is the
 * only thing config.txt carries, and it is written with the defaults on first
 * run so the choices are discoverable.
 *
 * MIT licensed, see LICENSE.
 */
#ifndef TPK_CONFIG_H
#define TPK_CONFIG_H

enum { TPK_RES_AUTO = 0, TPK_RES_720 = 720, TPK_RES_1080 = 1080 };
enum { TPK_LAYOUT_LABEL = 0, TPK_LAYOUT_POSITION = 1 };
enum { TPK_DPAD_HAT = 0, TPK_DPAD_STICK = 1, TPK_DPAD_BOTH = 2 };

/* The game's on-screen controls. Their positions are fixed in input.c. */
enum {
    CTL_NONE = 0, CTL_LEFT, CTL_RIGHT, CTL_SWAP, CTL_SWORD, CTL_JUMP,
    CTL_PAUSE, CTL_BACK, CTL_COUNT
};

/* What a player can press. */
enum {
    IN_A, IN_B, IN_X, IN_Y, IN_L, IN_R, IN_ZL, IN_ZR, IN_PLUS, IN_MINUS,
    IN_DLEFT, IN_DRIGHT, IN_DUP, IN_DDOWN,
    IN_SLEFT, IN_SRIGHT, IN_SUP, IN_SDOWN, IN_COUNT
};

typedef struct {
    /* the one editable setting */
    unsigned char map[IN_COUNT];   /* CTL_* for each input */

    /* fixed at build time */
    int log_level;
    int resolution;
    int button_layout;
    int dpad;
    int stick_deadzone;
    int touch;
    int touch_mouse;
    int rumble;
    int exit_combo;
    int minus_back_key;
    int dpi;
    int game_stack_mb;
    int gamepad;
    int vsync;
    int frame_stats;
    int gc_working_mb;
    int gc_free_mb;
    int game_core;
    int pad_reconnects;
    int touch_buttons;
    int pointer;
    int pointer_speed;
} TpkConfig;

extern TpkConfig g_cfg;

void config_defaults(void);
/* Reads the button mapping; writes the file with the defaults if absent. */
int  config_load(const char *path);

const char *config_input_name(int input);
const char *config_control_name(int control);

#endif
