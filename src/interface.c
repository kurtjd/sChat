/******************************************************************************
 * interface.c: Implementation of interface functions.                        *
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
#include <ctype.h>
#include <time.h>
#include "xcurses.h"
#include "interface.h"
#include "txtfield.h"
#include "scrollpane.h"
#include "message.h"
#include "linkedlist.h"
#include "helper.h"

void init_curses(void)
{
    initscr();
    cbreak();
    noecho();  // Gotta turn @echo off!
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);  // Makes getch() non-blocking.
}

int handle_input(LinkedList *messages, ScrollPane *sp, TxtField *tf)
{
    if (messages == NULL || sp == NULL || tf == NULL)
        return 0;

    int keyp = getch();

    switch (keyp) {
    case ERR:  // No key pressed.
        return -1;

    case '\n':  // This means the enter key was pressed.
        // Don't send blank messages.
        if (tf->length < 1)
            break;

        // Just a quick and temporary solution to allow for clean exit.
        if (strcmp(tf->value, "/q") == 0)
            clean_exit(EXIT_SUCCESS, messages, sp, tf);

        // Add the message to history then print it.
        Message *msg = msg_new(messages, FROM_SELF, time(0), tf->value);
        if (msg == NULL || !msg_print(msg, sp))
            return 0;

        msg_cycle_sent(messages, tf, 0);  // Resets message cycle.

        tf_clear(tf);
        break;

    // There are multiple keys representing backspace.
    case KEY_BACKSPACE:
    case 127:
    case 8:
        tf_backspace(tf);
        break;
    case KEY_C_UP:
        sp_scroll(sp, 1);
        break;
    case KEY_C_DOWN:
        sp_scroll(sp, -1);
        break;
    case KEY_LEFT:
        tf_move_cursor(tf, -1);
        break;
    case KEY_RIGHT:
        tf_move_cursor(tf, 1);
        break;
    case KEY_UP:
        msg_cycle_sent(messages, tf, 1);
        break;
    case KEY_DOWN:
        msg_cycle_sent(messages, tf, -1);
        break;
    case KEY_RESIZE:
        window_resize();
        tf_scale(tf, 0, LINES - 1, COLS);
        break;

    /* If we get here, a key was pressed that we don't explicitly want to handle.
     * However if a printable key was pressed, we want to handle that. */
    default:
        if (isprint(keyp))
            tf_insert(tf, keyp);
        break;
    }

    return 1;
}

void window_resize(void)
{
    endwin();
    refresh();
    clear();
}
