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
#include "txtfield.h"
#include "scrollpane.h"
#include "linkedlist.h"
#include "helper.h"

struct Message {
    unsigned sender;  // Either FROM_SELF or FROM_PEER.
    time_t timestamp;
    char txt[MAX_MSG_LEN];  // This will probably be dynamically allocated in the future.
};

// Combines all the elements of a message into a string. Only used internally hence the static qualifier.
static char* msg_format(const unsigned sender, const time_t timestamp, const char *msg)
{
    (void)timestamp;  // SUPPRESS UNUSED PARAMETER WARNING ONLY FOR NOW!

    // This will need to be dynamically allocated once I use names instead of the below defaults.
    char msg_sender[7];
    strncpy(msg_sender, (sender == FROM_SELF) ? "You:  " : "Peer: ", 7);

    /* Allocate enough space to hold the name along with the message.
    * +1 to account for null character. */
    size_t msglen = strlen(msg_sender) + strlen(msg) + 1;  // +1 for \n
    char *message = malloc(msglen + 1);
    if (message == NULL)
        return NULL;

    message[0] = '\0';

    // Then copy individual strings into message.
    strncpy(message, msg_sender, sizeof msg_sender);
    strncat(message, msg, strlen(msg) + 1);  // +1 for null char
    message[msglen - 1] = '\n';
    message[msglen] = '\0';

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

void msg_cycle_sent(LinkedList *messages, TxtField *tf, const int dir)
{
    if (messages == NULL || tf == NULL)
        return;

    static unsigned prev_msg_on = 0;

    // Cycle between previously sent messages.
    if (!dir) {
        prev_msg_on = 0;  // Reset message cycle.
        return;
    } else if (dir > 0) {
        if (prev_msg_on < messages->size)
            ++prev_msg_on;
        else
            prev_msg_on = 0;
    } else {
        if (prev_msg_on >= 1)
            --prev_msg_on;
        else
            prev_msg_on = messages->size;
    }

    // Clear the message buffer and text in the input field.
    tf_clear(tf);

    /* Loop through the previous messages until we find the one that matches what the user wants,
     * and then place it into the message buffer. */
    Message *msg = list_prev(messages);
    for (unsigned i = 0; i != prev_msg_on; msg = list_prev(messages)) {
        if (msg->sender == FROM_SELF)
            ++i;
        if (i == prev_msg_on)
            tf_set(tf, msg->txt);
    }
    list_iter_reset(messages);
    
    // Need to reset echo_start in case the message is longer than the screen.
    tf_reset_echo(tf);
}
