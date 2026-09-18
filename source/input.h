/* input.h -- controller and touch input, synthesized from libnx.
 *
 * WHAT THE GAME EXPECTS
 * Total Party Kill reads Lime *joystick* events (not gamepad events), with
 * indices in the XInput raw-joystick layout its "Gamepad Translator" table
 * names:
 *
 *   buttons  0 A  1 B  2 X  3 Y  4 LB  5 RB  6 Back  7 Start  8 LS  9 RS  10 Home
 *   axes     0/1 left stick  3/4 right stick  2 LT  5 RT (negative = pressed)
 *   hat 0    D-pad
 *
 * and its default controls are jump=0, swap=1, shoot=2, pause=7, movement on
 * hat 0 or axes 0/1, all on joystick id 0.
 *
 * WHAT switch-sdl2 PROVIDES
 * 28 buttons in Nintendo order, 4 axes, no hats, 8 always-present devices,
 * and no JOYDEVICEADDED event ever -- Lime only opens a joystick when that
 * event arrives. Its touch driver also mislabels finger IDs on motion.
 *
 * WHAT THIS DOES
 * sdl_bridge.c drops SDL's own joystick, controller, finger and touch-mouse
 * events. This file reads the pad and touch screen through libnx and pushes
 * the events the game expects: one JOYDEVICEADDED for device 0, joystick
 * buttons/axes/hat in the layout above, correct finger events, and (like SDL
 * on Android) a mouse click for the first finger.
 *
 *   button_layout = label     A->0 B->1 X->2 Y->3   (A jumps)
 *   button_layout = position  B->0 A->1 Y->2 X->3   (bottom button jumps)
 *
 * MIT licensed, see LICENSE.
 */
#ifndef TPK_INPUT_H
#define TPK_INPUT_H

#include <SDL2/SDL.h>

void input_init(void);
/* After SDL_Init / SDL_InitSubSystem: announce joystick 0 once. */
void input_on_sdl_init(void);
/* Poll libnx and push changed state as SDL events. Cheap; call often. */
void input_pump(void);
/* True while this thread is pushing a synthesized event. */
int  input_is_injecting(void);

/* Joystick queries for instance 0 are answered from the synthesized state. */
int  input_is_virtual(SDL_Joystick *joy);
int  input_num_buttons(void);
int  input_num_axes(void);
int  input_num_hats(void);
Uint8  input_button(int button);
Sint16 input_axis(int axis);
Uint8  input_hat(int hat);

/* Rumble for Android vibrate(ms). */
void input_vibrate(int ms);

#endif
