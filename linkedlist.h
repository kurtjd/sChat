/******************************************************************************
 * linkedlist.h: A generic, doubly linked-list.                               *
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
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdlib.h>

struct Node;
typedef struct Node Node;
struct Node
{
    Node *next;
    Node *prev;
    void *value;
};

typedef struct
{
    size_t maxsize;
    size_t size;
    Node *first;
    Node *last;
} LinkedList;

// Initialize linked list. A maxsize of 0 means unlimited size.
void list_init(LinkedList *list, const size_t maxsize);

// Adds a node to the linked list, then returns a pointer to it.
Node* list_append(LinkedList *list, void *value);

/* Frees the first node, then sets it to the second node.
 * Returns a pointer to the new beginning of the linked list. */
Node* list_prepop(LinkedList *list);

// Frees all memory in the linked list.
void list_clear(LinkedList *list);

#endif
