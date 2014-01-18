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

int handle_input(LinkedList *messages, ScrollPane *sp, TxtField *tf, unsigned *prev_msg_on)
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

        *prev_msg_on = 0;
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
        cycle_sent_msg(1, messages, tf, prev_msg_on);
        break;

    case KEY_DOWN:
        cycle_sent_msg(-1, messages, tf, prev_msg_on);
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

// This needs to go somewhere else. It is only temporary.
void cycle_sent_msg(const int dir, const LinkedList *messages, TxtField *tf, unsigned *prev_msg_on)
{
    if (messages == NULL || tf == NULL)
        return;

    // Cycle between previously sent messages.
    if (dir < 0) {
        if (*prev_msg_on >= 1)
            --*prev_msg_on;
        else
            *prev_msg_on = messages->size;
    } else if (dir > 0) {
        if (*prev_msg_on < messages->size)
            ++*prev_msg_on;
        else
            *prev_msg_on = 0;
    }

    // Clear the message buffer and text in the input field.
    tf_clear(tf);

    /* Loop through the previous messages until we find the one that matches what the user wants,
     * and then place it into the message buffer. */
    Node *node = messages->last;
    for (unsigned i = 0; i != *prev_msg_on; node = node->prev) {
        const Message *msg = node->value;
        if (msg->sender == FROM_SELF)
            ++i;
        if (i == *prev_msg_on)
            tf_set(tf, msg->txt);
    }
    
    // Need to reset echo_start in case the message is longer than the screen.
    tf_reset_echo(tf);
}
