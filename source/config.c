/* config.c -- see config.h. MIT licensed, see LICENSE. */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "config.h"
#include "log.h"

TpkConfig g_cfg;

static const char *const INPUT_NAMES[IN_COUNT] = {
    "a", "b", "x", "y", "l", "r", "zl", "zr", "plus", "minus",
    "dpad_left", "dpad_right", "dpad_up", "dpad_down",
    "stick_left", "stick_right", "stick_up", "stick_down",
};

static const char *const CONTROL_NAMES[CTL_COUNT] = {
    "none", "left", "right", "swap", "sword", "jump", "pause", "back",
};

const char *config_input_name(int i)   { return (i >= 0 && i < IN_COUNT) ? INPUT_NAMES[i] : "?"; }
const char *config_control_name(int c) { return (c >= 0 && c < CTL_COUNT) ? CONTROL_NAMES[c] : "?"; }

void config_defaults(void)
{
    int i;
    for (i = 0; i < IN_COUNT; i++)
        g_cfg.map[i] = CTL_NONE;
    g_cfg.map[IN_A]      = CTL_JUMP;     /* the up-arrow button, bottom right */
    g_cfg.map[IN_B]      = CTL_SWORD;
    g_cfg.map[IN_Y]      = CTL_SWAP;
    g_cfg.map[IN_PLUS]   = CTL_PAUSE;
    g_cfg.map[IN_MINUS]  = CTL_BACK;
    g_cfg.map[IN_DLEFT]  = CTL_LEFT;
    g_cfg.map[IN_DRIGHT] = CTL_RIGHT;
    g_cfg.map[IN_SLEFT]  = CTL_LEFT;
    g_cfg.map[IN_SRIGHT] = CTL_RIGHT;

    /* Fixed. These were settings once; they are decided now. */
    g_cfg.log_level      = TPK_LOG_OFF;
    g_cfg.resolution     = TPK_RES_AUTO;   /* 720p handheld, 1080p docked */
    g_cfg.button_layout  = TPK_LAYOUT_LABEL;
    g_cfg.dpad           = TPK_DPAD_HAT;
    g_cfg.stick_deadzone = 7000;
    g_cfg.touch          = 1;
    g_cfg.touch_mouse    = 1;
    g_cfg.rumble         = 1;
    g_cfg.exit_combo     = 0;              /* no quit chord: use HOME */
    g_cfg.minus_back_key = 0;
    g_cfg.dpi            = 160;
    g_cfg.game_stack_mb  = 16;
    g_cfg.gamepad        = 1;
    g_cfg.vsync          = 1;
    g_cfg.frame_stats    = 0;
    g_cfg.gc_working_mb  = 64;
    g_cfg.gc_free_mb     = 24;
    g_cfg.game_core      = -1;
    g_cfg.pad_reconnects = 4;
    g_cfg.touch_buttons  = 1;
    g_cfg.pointer        = 1;
    g_cfg.pointer_speed  = 1100;
}

static const char DEFAULT_TEXT[] =
"# Total Party Kill -- Nintendo Switch port\n"
"#\n"
"# Which Switch input presses which of the game's on-screen controls.\n"
"# Everything else about the port is fixed.\n"
"#\n"
"# Controls you can assign:\n"
"#   jump    the up-arrow button, bottom right\n"
"#   sword   the sword button\n"
"#   swap    the character button, bottom middle\n"
"#   left    the left arrow\n"
"#   right   the right arrow\n"
"#   pause   the pause button, top right\n"
"#   back    the back arrow, top left\n"
"#   none    nothing\n"
"#\n"
"# ZL and ZR together always raise the on-screen cursor, whatever they are\n"
"# assigned to here. Delete this file to restore the defaults.\n"
"\n"
"a = jump\n"
"b = sword\n"
"x = none\n"
"y = swap\n"
"l = none\n"
"r = none\n"
"zl = none\n"
"zr = none\n"
"plus = pause\n"
"minus = back\n"
"\n"
"dpad_left = left\n"
"dpad_right = right\n"
"dpad_up = none\n"
"dpad_down = none\n"
"\n"
"stick_left = left\n"
"stick_right = right\n"
"stick_up = none\n"
"stick_down = none\n";

static char *trim(char *s)
{
    char *e;
    while (*s && isspace((unsigned char)*s))
        s++;
    e = s + strlen(s);
    while (e > s && isspace((unsigned char)e[-1]))
        *--e = '\0';
    return s;
}

static void apply(const char *k, const char *v)
{
    int i, c;

    for (i = 0; i < IN_COUNT; i++) {
        if (strcasecmp(k, INPUT_NAMES[i]) != 0)
            continue;
        for (c = 0; c < CTL_COUNT; c++) {
            if (!strcasecmp(v, CONTROL_NAMES[c])) {
                g_cfg.map[i] = (unsigned char)c;
                return;
            }
        }
        LOGI("config: '%s' is not a control name, leaving %s as %s", v,
             INPUT_NAMES[i], CONTROL_NAMES[g_cfg.map[i]]);
        return;
    }
    /* Undocumented, for working out why something misbehaves: log_level = 1
     * (or 2) turns tpk.log back on. */
    if (!strcasecmp(k, "log_level")) {
        g_cfg.log_level = atoi(v);
        return;
    }
    LOGI("config: unknown key '%s'", k);
}

int config_load(const char *path)
{
    char line[256];
    FILE *fp = fopen(path, "r");
    if (!fp) {
        fp = fopen(path, "w");
        if (fp) {
            fputs(DEFAULT_TEXT, fp);
            fclose(fp);
        }
        return 0;
    }
    while (fgets(line, sizeof(line), fp)) {
        char *s = trim(line), *eq;
        if (!*s || *s == '#' || *s == ';')
            continue;
        eq = strchr(s, '=');
        if (!eq)
            continue;
        *eq = '\0';
        apply(trim(s), trim(eq + 1));
    }
    fclose(fp);
    return 1;
}
