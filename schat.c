/******************************************************************************
 * sChat: Peer-to-peer encrypted chat program.                                *
 * Copyright (C) 2014  Kurtis Dinelle & McConnell Wade                        *
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
#include <stdio.h>
#include <locale.h>
#include "xcurses.h"
#include "interface.h"
#include "txtfield.h"
#include "scrollpane.h"
#include "network.h"
#include "message.h"
#include "linkedlist.h"
#include "helper.h"

// Show usage banner
void show_banner(void)
{
    printf("sChat v0.1\nUsage: ./schat [-flags] peer\n");
}

// Performs cleanup then exits program.


int main()
{
    setlocale(LC_ALL, "");  // Once I get Unicode figured out...
    init_curses();

    unsigned prev_msg_on = 0;  // Keeps track as the user cycles through sent messages.

    LinkedList messages;
    list_init(&messages, 0);

    ScrollPane chatpane;
    TxtField input;

    // Initialize the chatpane and input field, while checking for errors.
    if (!sp_init(&chatpane, 0, 0, COLS, LINES - 1) || !tf_init(&input, 0, LINES - 1, COLS, MAX_MSG_LEN))
        clean_exit(EXIT_FAILURE, NULL, NULL, NULL);
    
    // This is where the magic happens.
    while (1) {
        tf_draw(&input);

        // If anything throws an error, exit gracefully.
        if (!handle_input(&messages, &chatpane, &input, &prev_msg_on))
            clean_exit(EXIT_FAILURE, &messages, &chatpane, &input);

        doupdate();  // Update all windows at once rather than individually.
    }

    clean_exit(EXIT_SUCCESS, &messages, &chatpane, &input);  // Should never get to here...
}
