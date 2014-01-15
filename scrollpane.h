#ifndef SCROLLPANE_H
#define SCROLLPANE_H

#include <string.h>
#include "xcurses.h"

typedef struct
{
    unsigned XPOS;
    unsigned YPOS;
    size_t WIDTH;
    size_t HEIGHT;

    unsigned scroll_offset;

    WINDOW *win;
} ScrollPane;

// Creates a new window and sets defaults.
void sp_init(ScrollPane *sp, const unsigned xpos, const unsigned ypos, const unsigned width, const unsigned height);

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
