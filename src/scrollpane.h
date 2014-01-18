/******************************************************************************
 * scrollpane.h: A generic, scrollable ncurses window.                        *
 * Copyright (C) 2014  Kurtis Dinelle & McConnell Wade                        *
 *                                                                            *
 * This file is a part of sChat.                                              *
 *                                                                            *
 * sChat is free software: you can redistribute it and/or modify              *
 * it under the terms of the GNU General Public License as published by       *
 * the Free Software Foundation, either version 3 of the License, or          *
 * (at your option) any later version.                                        *
 *                                                                            *
 * sChat is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with sChat.  If not, see [http://www.gnu.org/licenses/].             *
 ******************************************************************************/
#ifndef SCROLLPANE_H
#define SCROLLPANE_H

#include <stdlib.h>
#include "xcurses.h"
#include "linkedlist.h"

typedef struct {
    unsigned x;
    unsigned y;
    size_t width;
    size_t height;

    LinkedList lines;  // A linked list where each individual line is a node.
    unsigned scroll_offset;  // Keeps track as the user scrolls the window.

    WINDOW *win;  // An ncurses window.
} ScrollPane;

// Creates a new window and sets defaults. Returns 0 on failure.
int sp_init(ScrollPane *sp, const unsigned x, const unsigned y, const size_t width, const size_t height);

// Deallocates the window memory.
void sp_destroy(ScrollPane *sp);

// Adds each line of the given string to the linked-list. Returns 0 if a line could not be appended.
int sp_print(ScrollPane *sp, const char *txt);

// Scrolls the window.
void sp_scroll(ScrollPane *sp, const int dir);

// Clears the window and resets some other values.
void sp_reset(ScrollPane *sp);

#endif
