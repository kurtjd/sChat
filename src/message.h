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
#include "txtfield.h"
#include "scrollpane.h"
#include "linkedlist.h"

// These need to go away somehow.
#define MAX_MSG_LEN 500
#define FROM_SELF 0
#define FROM_PEER 1

typedef struct Message Message;

// Creates a new instance of the Message struct on the heap, then adds it to the message list. Returns NULL on failure.
Message* msg_new(LinkedList *messages, const unsigned sender, const time_t timestamp, const char *msg);

// Prints a single message to the chatpane. Returns 0 on failure to allocate memory.
int msg_print(const Message *message, ScrollPane *sp);

/* Copies a previously sent message into the text field.
 * A dir of 0 resets the message cycle. */
void msg_cycle_sent(LinkedList *messages, TxtField *tf, const int dir);

#endif
