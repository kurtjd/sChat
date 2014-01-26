/******************************************************************************
 * txtfield.c: Implementation of the text field container.                    *
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
#include <stdlib.h>
#include <string.h>
#include "xcurses.h"
#include "txtfield.h"
#include "helper.h"

// These calculations are used multiple times, so they are better off as macros.
#define ECHO_OFFSET (tf->scroll_gap * tf->echo_start)
#define TF_CURSOR_POS (get_cursor(X) - tf->x)

int tf_init(TxtField *tf, const unsigned x, const unsigned y, const size_t width, const size_t maxlen)
{
    if (tf == NULL)
        return 0;

    tf->height = 1;

    tf->maxlen = maxlen;
    tf->scroll_gap = 20;  // May make this dynamic in the future!

    tf->value = malloc(maxlen + 1);
    if (tf->value == NULL)
        return 0;

    tf->value[0] = '\0';
    tf->length = 0;
    tf->echo_start = 0;
    tf->cursor_offset = 0;

    tf_scale(tf, x, y, width);

    return 1;
}

void tf_destroy(TxtField *tf)
{
    if (tf == NULL)
        return;

    free(tf->value);
    tf->value = NULL;
}

void tf_insert(TxtField *tf, const char c)
{
    if (tf == NULL || tf->length >= (tf->maxlen - 1))
        return;

    // The index in the text buffer c is to be inserted.
    unsigned insert_at = TF_CURSOR_POS + ECHO_OFFSET;

    // Move characters to the right that are after the insertion point.
    for (size_t i = tf->length; i > insert_at; --i)
        tf->value[i] = tf->value[i - 1];

    tf->value[insert_at] = c;
    tf->value[tf->length + 1] = '\0';
    ++tf->length;

    if (get_cursor(X) == (tf->x + tf->width - 1))
        tf_reset_echo(tf);
}

void tf_set(TxtField *tf, const char *value)
{
    if (tf == NULL)
        return;

    tf->length = strlen(value);
    strncpy(tf->value, value, tf->length + 1);
}

void tf_backspace(TxtField *tf)
{
    // Don't want this function running if there is no input.
    if (tf == NULL || tf->length < 1)
        return;

    // The index in the message buffer to perform deletion.
    unsigned delete_at = (TF_CURSOR_POS - 1) + ECHO_OFFSET;

    for (size_t i = delete_at; i < tf->length; ++i)
        tf->value[i] = tf->value[i + 1];

    moveby(0, -1);
    delch();
    --tf->length;

    if (get_cursor(X) <= tf->x)
        tf_reset_echo(tf);
        /* Remember: This can cause prompt to be removed by tf_backspace if cursor offset is not 0.
         * Will fix after fixing the issue with inserting text in the middle of the message. */
}

void tf_clear(TxtField *tf)
{
    if (tf == NULL)
        return;

    move(tf->y, tf->x);

    clrtoeol();

    tf->echo_start = 0;
    tf->cursor_offset = 0;

    tf->value[0] = '\0';
    tf->length = 0;
}

void tf_draw(TxtField *tf)
{
    if (tf == NULL)
        return;

    tf_draw_border(tf);

    move(tf->y, tf->x);
    printw("%s", tf->value + ECHO_OFFSET);

    // Moves the cursor back to it's offset position if the user has moved it.
    tf_move_cursor(tf, 0);
}

void tf_reset_echo(TxtField *tf)
{
    if (tf == NULL)
        return;

    move(tf->y, tf->x);
    clrtoeol();
    tf->echo_start = 0;

    /* Basically calculates how many times the user would've typed past the width
     * of the text field, and then sets echo_start to that value. */
    if (tf->length >= tf->width)
        tf->echo_start = 1 + ((tf->length - (tf->width)) / tf->scroll_gap);
}

void tf_move_cursor(TxtField *tf, const int dir)
{
    if (tf == NULL)
        return;

    if (dir < 0 && get_cursor(X) > tf->x)
        --tf->cursor_offset;
    else if (dir > 0 && tf->cursor_offset < 0)
        ++tf->cursor_offset;

    move(tf->y, get_cursor(X) + tf->cursor_offset);
}

void tf_scale(TxtField *tf, const unsigned newx, const unsigned newy, const size_t newwidth)
{
    if (tf == NULL)
        return;

    tf->x = newx;
    tf->y = newy;
    tf->width = newwidth;

    tf_reset_echo(tf);
}

// Will probably get rid of this in the future.
void tf_draw_border(const TxtField *tf)
{
    if (tf == NULL)
        return;

    move(tf->y - 1, tf->x);

    for (size_t i = 0; i < tf->width; ++i)
        addch('_');
}
