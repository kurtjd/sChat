/* A collection of functions that handle the actual interface and I/O */
#ifndef INTERFACE_H
#define INTERFACE_H

#include "txtfield.h"
#include "message.h"

// Sets up the curses environment with preferred settings.
void init_curses(void);

// Displays all messages in history.
void show_message_history(const MessageHistory *messages, int *hist_start);

// Handles all user input.
void handle_input(TxtField *txtbox, MessageHistory *messages, int *hist_start, int *prev_msg_on);

// Prints a message line-by-line upwards.
void print_lines(const char *msg, const int msglines, int *starty, int *line_on, const int hist_start);

// Prints a message while also checking if the screen is full.
void print_message(char *msg, const int msglines, const int maxlines, int *screenfull, int *starty);

// Reinitializes ncurses to work with the resized window.
void window_resize(void);

// Changes which message in the history to begin printing from.
void change_hist_start(const int dir, int *hist_start, const MessageHistory *messages);

// Returns the total number of lines of all messages in the history.
int get_hist_lines_total(const MessageHistory *messages);

// Copies a previously sent message into the text field.
void cycle_sent_msg(const int dir, const MessageHistory *messages, TxtField *txtbox, int *prev_msg_on);

#endif
