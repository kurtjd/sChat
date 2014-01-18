/******************************************************************************
 * scrollpane.c: Implementation of a scrollpane window.                       *
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
#include <string.h>
#include "xcurses.h"
#include "txtfield.h"
#include "scrollpane.h"
#include "linkedlist.h"
#include "helper.h"

// Given a position in a string, creates a new string that would fit on one line.
static char* sp_new_line(ScrollPane *sp, const char *txt, const size_t lstart)
{
    // Determines where in the string a new line would start, and now long this line should be.
    const char *line_start = txt + (lstart * sp->width);
    const size_t linelen = strlen(line_start) >= sp->width ? sp->width : strlen(line_start);

    char *line = malloc(linelen + 1);
    if (line == NULL)
        return NULL;

    strncpy(line, line_start, linelen);
    line[linelen] = '\0';

    if (list_append(&sp->lines, line) == NULL)
        return NULL;

    return line;
}

int sp_init(ScrollPane *sp, const unsigned x, const unsigned y, const size_t width, const size_t height)
{
    if (sp == NULL)
        return 0;

    sp->x = x;
    sp->y = y;

    // Because ncurses pushes cursor to next line if a line is equal to the window width.
    sp->width = width - 1;
    sp->height = height;

    list_init(&sp->lines, 0);
    sp->scroll_offset = 0;

    sp->win = newwin(height, width, y, x);
    if (sp->win == NULL)
        return 0;

    scrollok(sp->win, TRUE);  // Allows text to scroll in the window when hitting the bottom.

    return 1;
}

void sp_destroy(ScrollPane *sp)
{
    if (sp == NULL)
        return;

    delwin(sp->win);
    sp->win = NULL;
    list_clear(&sp->lines);
}

int sp_print(ScrollPane *sp, const char *txt)
{
    if (sp == NULL)
        return 0;
    
    // Breaks the string into lines and appends each line to the linked-list, then prints it.
    const unsigned linec = (strlen(txt) / sp->width) + 1;
    for (unsigned i = 0; i < linec; ++i) {
        char *line = sp_new_line(sp, txt, i);
        if (line == NULL)
            return 0;

        /* If the user is scrolling upwards, don't attempt to print text.
         * Instead, just increase the scroll offset to account for
         * the newly added line. */
        if (!sp->scroll_offset)
            wprintw(sp->win, "%s\n", line);
        else
            ++sp->scroll_offset;
    }

    wnoutrefresh(sp->win);

    return 1;
}

void sp_scroll(ScrollPane *sp, const int dir)
{
    // Function shouldn't run if the screen isn't full or the user tries to scroll down past the last line.
    if (sp == NULL || !dir || list_size(&sp->lines) < sp->height || (dir < 0 && !sp->scroll_offset))
        return;

    sp->scroll_offset += dir;

    unsigned starty = sp->y;
    int line_index;

    if (dir > 0) {
        line_index = (list_size(&sp->lines) - sp->height - sp->scroll_offset) + 1;

        // Prevents user from scrolling up past the first line.
        if (line_index < 0) {
            sp->scroll_offset -= dir;  // Undo the change to scroll_offset.
            return;
        }

        wscrl(sp->win, -dir);
    } else {
        starty += (sp->height - 1);
        line_index = list_size(&sp->lines) - sp->scroll_offset - 1;
    }

    /* Either prints a line at the top or bottom of window, depending on which way
     * the user scrolled. If the user scrolled down, the line is placed in the blank
     * area at the bottom of the window, and a newline is printed to actually
     * cause the window to scroll. */
    mvwprintw(sp->win, starty, sp->x, "%s", list_get(&sp->lines, line_index));
    if (dir < 0)
        waddch(sp->win, '\n');

    wnoutrefresh(sp->win);
}

void sp_reset(ScrollPane *sp)
{
    if (sp == NULL)
        return;

    wclear(sp->win);
    list_clear(&sp->lines);
}
