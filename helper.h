/* Just a collection of miscellaneous helper functions that don't need to
 * be cluttering the main source file. */
#ifndef HELPER_H
#define HELPER_H

#include <string.h>
#include "message.h"

// Appends char c to string dest.
void append(char dest[], const char c);

// Moves the cursor by specified amount.
void moveby(const int yinc, const int xinc);

// Clears the input buffer and the echo'd input from the screen.
void clear_input(char msgbuf[]);

// Deletes the last char from input buffer and from the screen.
void backspace(char msgbuf[]);

// Checks if malloc was successful and peforms cleanup if not.
void* safe_malloc(const size_t size, MessageHistory *messages);

// Performs cleanup then exits program.
void clean_exit(const int status, MessageHistory *messages);

#endif
