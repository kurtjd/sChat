/******************************************************************************
 * txtfield.h: Generic text field structure.                                  *
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
#ifndef TXTFIELD_H
#define TXTFIELD_H

#include <stdlib.h>

typedef struct
{
    unsigned x;
    unsigned y;
    size_t width;
    size_t height;

    size_t maxlen;
    size_t length;
    size_t scroll_gap;  // How much space the textfield should scroll by when input reaches width.

    char *value;

    unsigned echo_start;  // The position in the input to begin echoing.
    int cursor_offset;  // Cursor position relative to end of input.
} TxtField;

// Initializes a new text field. Returns 0 if memory could not be allocated.
int tf_init(TxtField *tf, const unsigned x, const unsigned y, const size_t width, const size_t maxlen);

// Performs cleanup.
void tf_destroy(TxtField *tf);

// Inserts a character into the textfield at the current cursor position.
void tf_insert(TxtField *tf, const char c);

// Sets the contents of the textfield to a specified string.
void tf_set(TxtField *tf, const char *value);

// Deletes the char before the cursor from textfield and from the screen.
void tf_backspace(TxtField *tf);

// Clears the textfield and the echo'd input from the screen.
void tf_clear(TxtField *tf);

// Echo back what the user is currently typing.
void tf_draw(TxtField *tf);

// Recalculates the echo_start variable.
void tf_reset_echo(TxtField *tf);

// Moves the cursor for input. Called when the left or right key is pressed.
void tf_move_cursor(TxtField *tf, const int dir);

// Resizes and repositions the text field.
void tf_scale(TxtField *tf, const unsigned newx, const unsigned newy, const unsigned newwidth);

// Draws a horizontal line across the screen above the input field.
void tf_draw_border(const TxtField *tf);

#endif
