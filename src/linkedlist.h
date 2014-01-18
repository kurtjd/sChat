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

typedef struct Node Node;
typedef struct {
    size_t maxsize;
    size_t size;
    Node *first;
    Node *last;

    // To keep track if the user wishes to traverse through list.
    Node *next;
    Node *prev;
} LinkedList;

// Initialize linked list. A maxsize of 0 means unlimited size.
void list_init(LinkedList *list, const size_t maxsize);

/* Returns a pointer to the value at the specified index in the list.
 * Do NOT use to traverse a list, such as in a for loop!
 * Use list_next() or list_prev() instead. */
void* list_get(LinkedList *list, const size_t index);

/* Returns the value of the next node in the list.
 * If list->next is null, return the first node.
 * If list->next is the last node, return null.
 * Note: If used in a loop that may break before reaching end of list,
 * make sure to call list_iter_reset(). */
void* list_next(LinkedList *list);

/* Returns the value of the previous node in the list.
 * If list->prev is null, return the last node.
 * If list->prev is the first node, return null.
 * Note: If used in a loop that may break before reaching beginning of list,
 * make sure to call list_iter_reset(). */
void* list_prev(LinkedList *list);

// Adds a node to the linked list, then returns a pointer to its value.
void* list_append(LinkedList *list, void *value);

/* Frees the first node, then sets it to the second node.
 * Returns a pointer to the value of the new beginning of the linked list. */
void* list_prepop(LinkedList *list);

// Returns the size of the list.
size_t list_size(const LinkedList *list);

// Resets list->next and list->prev to NULL.
void list_iter_reset(LinkedList *list);

// Frees all memory in the linked list.
void list_clear(LinkedList *list);

#endif
