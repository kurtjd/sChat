/* A collection of functions that handle the actual interface and I/O */

#ifndef INTERFACE_H
#define INTERFACE_H

#define MAX_MSG_LEN 51
#define MAX_HISTORY 100

/* Sets up the curses environment with preferred settings.
 * screen_h and screen_w are set by this function.
 * Remember: arguments are constant pointers not pointers to
 * constant data. The data WILL change, but the pointers
 * will never point to anything else! */
void init_curses(int * const screen_h, int * const screen_w);

/* Draws a horizontal line across the screen, separating the message pane from the input 'field'.
 * Pass the screen height because the y position should be screen_h - X where X is some offset. */
void draw_input_field(const int length, const int screen_h);

// Echo back what the user is currently typing.
void echo_user_input(const char msgbuf[], const int screen_h, const int xstart);

// Handles all user input. Self-explanatory.
void handle_input(char msgbuf[]);

#endif
