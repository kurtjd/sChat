#ifndef INTERFACE_H
#define INTERFACE_H

#include "txtfield.h"
#include "scrollpane.h"
#include "linkedlist.h"

// Sets up the curses environment with preferred settings.
void init_curses(void);

// Handles all user input.
void handle_input(LinkedList *messages, ScrollPane *sp, TxtField *tf, unsigned *prev_msg_on);

// Reinitializes ncurses to work with the resized window.
void window_resize(void);

// Copies a previously sent message into the text field.
void cycle_sent_msg(const int dir, const LinkedList *messages, TxtField *tf, unsigned *prev_msg_on);

#endif
