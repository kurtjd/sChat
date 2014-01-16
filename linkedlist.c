/******************************************************************************
 * linkedlist.c: Implementation of the linked-list structure.                 *
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
#include "linkedlist.h"

// Creates a new node.
static Node* list_new_node(LinkedList *list, void *value)
{
    if (list == NULL)
        return NULL;

    Node *node = malloc(sizeof *node);
    if (node == NULL)
        return NULL;

    node->next = NULL;

    // If first node, this will be null as intended.
    node->prev = list->last;
    
    node->value = value;

    return node;
}

void list_init(LinkedList *list, const size_t maxsize)
{
    if (list == NULL)
        return;

    list->maxsize = maxsize;  // Hmmm what would a maxsize of 1 do?
    list->size = 0;

    // Set these pointers to NULL so any attempt to dereference prematurely will crash.
    list->first = NULL;
    list->last = NULL;
}

Node* list_append(LinkedList *list, void *value)
{
    if (list == NULL)
        return NULL;

    Node *node = list_new_node(list, value);
    if (node == NULL)
        return NULL;

    if (list->first == NULL)
        list->first = node;
    else if (list->maxsize && list->size >= list->maxsize)  // If at max size, make room for new node.
        list_prepop(list);

    // If this is the first node, we want next to remain NULL.
    if (list->size > 0)
        list->last->next = node;

    list->last = node;

    ++list->size;
    return node;
}

Node* list_prepop(LinkedList *list)
{
    if (list == NULL)
        return NULL;
    
    Node *nxtnode = list->first->next;
    nxtnode->prev = NULL;

    free(list->first);
    list->first = nxtnode;

    --list->size;
    return list->first;
}

void list_clear(LinkedList *list)
{
    if (list == NULL)
        return;

    Node *node = list->first;
    while (node) {
        Node *tmp = node->next;
        free(node->value);
        node->value = NULL;
        free(node);
        node = tmp;
    }

    list->size = 0;
}
