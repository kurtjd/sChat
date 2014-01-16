/******************************************************************************
 * message.c: Implementation of a message component.                          *
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
#include <time.h>
#include "message.h"
#include "linkedlist.h"
#include "scrollpane.h"
#include "helper.h"

// Combines all the elements of a message into a string. Only used internally hence the static qualifier.
static char* msg_format(const unsigned sender, const time_t timestamp, const char *msg)
{
    (void)timestamp;  // SUPPRESS UNUSED PARAMETER WARNING ONLY FOR NOW!

    // This will need to be dynamically allocated once I use names instead of the below defaults.
    char msg_sender[7];
    strncpy(msg_sender, (sender == FROM_SELF) ? "You:  " : "Peer: ", 7);

    /* Allocate enough space to hold the name along with the message.
    * +1 to account for null character. */
    char *message = malloc(strlen(msg_sender) + strlen(msg) + 1);
    if (message == NULL)
        return NULL;

    message[0] = '\0';

    // Then copy individual strings into message.
    strncpy(message, msg_sender, sizeof msg_sender);
    strncat(message, msg, strlen(msg) + 1);  // +1 for null char

    return message;
}

Message* msg_new(LinkedList *messages, const unsigned sender, const time_t timestamp, const char *msg)
{
    if (messages == NULL)
        return NULL;

    Message *new_msg = malloc(sizeof *new_msg);
    if (new_msg == NULL)
        return NULL;

    new_msg->sender = sender;
    new_msg->timestamp = timestamp;

    // Can't use sizeof on an array passed into a function, so add 1 to account for null char.
    strncpy(new_msg->txt, msg, strlen(msg) + 1);

    if (list_append(messages, new_msg) == NULL)
        return NULL;

    return new_msg;
}

int msg_print(const Message *message, ScrollPane *sp)
{
    if (message == NULL || sp == NULL)
        return 0;

    char *msg = msg_format(message->sender, message->timestamp, message->txt);

    if (msg == NULL || !sp_print(sp, msg))
        return 0;

    free(msg);
    msg = NULL;

    return 1;
}

// The below two functions will be going away once ScrollPane is complete.
int msg_print_all(const LinkedList *messages, ScrollPane *sp)
{
    if (messages == NULL || sp == NULL)
        return 0;

    sp_reset(sp);
    
    for (Node *node = messages->first; node != NULL; node = node->next) {
        if (!msg_print(node->value, sp))
            return 0;
    }

    return 1;
}

int msg_all_linec(const LinkedList *messages, const size_t screenw)
{
    if (messages == NULL)
        return 0;

    unsigned total_lines = 0;

    for (Node *node = messages->first; node != NULL; node = node->next) {
        const Message *msg = node->value;
        char *final_msg = msg_format(msg->sender, msg->timestamp, msg->txt);
        if (final_msg == NULL)
            return 0;

        total_lines += ((strlen(final_msg) / screenw) + 1);
        free(final_msg);
        final_msg = NULL;
    }

    return total_lines;
}
