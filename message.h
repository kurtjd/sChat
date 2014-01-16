/******************************************************************************
 * message.h: A structure used to organize a chat message.                    *
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
#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdlib.h>
#include <time.h>
#include "scrollpane.h"
#include "linkedlist.h"

// These need to go away somehow.
#define MAX_MSG_LEN 500
#define FROM_SELF 0
#define FROM_PEER 1

typedef struct
{
    unsigned sender;  // Either FROM_SELF or FROM_PEER.
    time_t timestamp;
    char txt[MAX_MSG_LEN];  // This will probably be dynamically allocated in the future.
} Message;

// Creates a new instance of the Message struct on the heap, then adds it to the message list. Returns NULL on failure.
Message* msg_new(LinkedList *messages, const unsigned sender, const time_t timestamp, const char *msg);

// Prints a single message to the chatpane. Returns 0 on failure to allocate memory.
int msg_print(const Message *message, ScrollPane *sp);

// Prints all messages to the chatpane. Returns 0 on failure.
int msg_print_all(const LinkedList *messages, ScrollPane *sp);

// Returns the total number of lines of all messages in the history. Returns 0 on failure.
int msg_all_linec(const LinkedList *messages, const size_t screenw);

#endif
