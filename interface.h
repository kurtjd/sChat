/* A collection of functions that handle the actual interface and I/O */
#ifndef INTERFACE_H
#define INTERFACE_H

#include "message.h"

#define PROMPT_LEN 2
#define SCROLL_GAP 20
#define INPUT_HEIGHT 2

/* Sets up the curses environment with preferred settings.
 * screen_h and screen_w are set by this function. */
void init_curses(int *screen_h, int *screen_w);

// Displays all messages in history.
void show_message_history(const MessageHistory *messages, const int screen_h, const int screen_w, int *hist_start);

/* Draws a horizontal line across the screen, separating the message pane from the input 'field'.
 * Pass the screen height because the y position should be screen_h - X where X is some offset. */
void draw_input_field(const int length, const int screen_h);

// Echo back what the user is currently typing.
void echo_user_input(const char *msgbuf, const unsigned screen_h, const int echo_start, int *cursor_offset);

// Changes the index to begin echoing input.
void change_echo_start(int *echo_start, const int dir, const int screen_w);

// Handles all user input.
void handle_input(char *msgbuf, MessageHistory *messages, int *screen_h, int *screen_w, int *echo_start,
                  int *cursor_offset, int *hist_start, int *prev_msg_on);

// Prints a message line-by-line upwards.
void print_lines(const char *msg, const int msglines, const int screen_w, int *starty, int *line_on, const int hist_start);

// Prints a message while also checking if the screen is full.
void print_message(char *msg, const int msglines, const int maxlines, int *screenfull, int *starty);

// Reinitializes ncurses to work with the resized window.
void window_resize(int *screen_h, int *screen_w);

// Clears the input buffer and the echo'd input from the screen.
void clear_input(char *msgbuf, int *echo_start, int *cursor_offset, int *prev_msg_on);

// Deletes the last char from input buffer and from the screen.
void backspace(char *msgbuf, const int echo_start);

// Moves the cursor for input. Called when the left or right key is pressed.
void move_cursor(const int dir, int *cursor_offset, const int xmax, const int echo_start);

// Inserts a character into the message buffer at the current cursor position.
void insert_char(char *msgbuf, const char c, const int echo_start);

// Changes which message in the history to begin printing from.
void change_hist_start(const int dir, int *hist_start, const int screen_h, const int screen_w, const MessageHistory *messages);

// Returns the total number of lines of all messages in the history.
int get_hist_lines_total(const MessageHistory *messages, const int screen_w);

// Copies a previously sent message into the current message buffer.
void cycle_sent_msg(const int dir, const MessageHistory *messages, char *msgbuf, int *prev_msg_on,
                    int *echo_start, const int screen_w, int *cursor_offset);

// Does some checking before calling insert_char().
void add_to_msg(char *msgbuf, const char c, int *echo_start, const int screen_w);

#endif
