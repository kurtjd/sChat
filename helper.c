/******************************************************************************
 * helper.c: Implementation of helper functions.                              *
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
#include "interface.h"
#include "txtfield.h"
#include "scrollpane.h"
#include "linkedlist.h"
#include "helper.h"

void moveby(const int yinc, const int xinc)
{
    unsigned cury, curx;
    getyx(stdscr, cury, curx);  // Gets current cursor position.
    move(cury + yinc, curx + xinc);
}

void clean_exit(const int status, LinkedList *messages, ScrollPane *sp, TxtField *tf)
{
    if (messages)
        list_clear(messages);
    if (sp)
        sp_destroy(sp);
    if (tf)
        tf_destroy(tf);

    endwin();
    exit(status);
}

unsigned get_cursor(const unsigned axis)
{
    unsigned cury, curx;
    getyx(stdscr, cury, curx);

    return (axis == Y) ? cury : curx;
}
