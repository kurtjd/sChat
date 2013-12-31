/* A collection of functions that handle the actual interface and I/O */
#ifndef INTERFACE_H
#define INTERFACE_H

#include "message.h"

#define PROMPT_LEN 2
#define INPUT_HEIGHT 2

/* Sets up the curses environment with preferred settings.
 * screen_h and screen_w are set by this function. */
void init_curses(int *screen_h, int *screen_w);

// Displays all messages in history.
void show_message_history(const MessageHistory *messages, const int screen_h, const int screen_w);

/* Draws a horizontal line across the screen, separating the message pane from the input 'field'.
 * Pass the screen height because the y position should be screen_h - X where X is some offset. */
void draw_input_field(const int length, const int screen_h);

// Echo back what the user is currently typing.
void echo_user_input(const char *msgbuf, const int screen_h, const int screen_w, const int xstart);

// Handles all user input.
void handle_input(char *msgbuf, MessageHistory *messages, const unsigned screen_w);

// Prints a message line-by-line upwards.
void print_lines(const char *msg, const int msglines, const int screen_w, int *starty);

// Prints a message while also checking if the screen is full.
void print_message(char *msg, const int msglines, const int maxlines, int *screenfull,
                   int *starty, const MessageHistory *messages, const int screen_h, const int screen_w);

#endif
