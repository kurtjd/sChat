#ifndef SCROLLPANE_H
#define SCROLLPANE_H

#include <stdlib.h>
#include "xcurses.h"

typedef struct
{
    unsigned x;
    unsigned y;
    size_t width;
    size_t height;

    unsigned scroll_offset;  // Keeps track as the user scrolls the window.

    WINDOW *win;  // An ncurses window.
} ScrollPane;

// Creates a new window and sets defaults. Returns 0 on failure.
int sp_init(ScrollPane *sp, const unsigned x, const unsigned y, const size_t width, const size_t height);

// Deallocates the window memory.
void sp_destroy(ScrollPane *sp);

// Prints some text to the window and refeshes.
void sp_print(ScrollPane *sp, const char *txt);

// Prints each line of a body of text sequentially.
void sp_print_lines(ScrollPane *sp, const char *txt);

// Scrolls the window.
void sp_scroll(ScrollPane *sp, const int dir);

// Clears the window and resets some other values.
void sp_reset(ScrollPane *sp);

#endif
