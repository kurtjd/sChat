/* Just a collection of miscellaneous helper functions that don't need to
 * be cluttering the main source file. */

#ifndef HELPER_H
#define HELPER_H

// Appends char c to string dest.
void append(char dest[], const char c);

// Moves the cursor by specified amount.
void moveby(const int yinc, const int xinc);

// Clears the input buffer and the echo'd input from the screen.
void clear_input(char msgbuf[]);

// Deletes the last char from input buffer and from the screen.
void backspace(char msgbuf[]);

#endif
