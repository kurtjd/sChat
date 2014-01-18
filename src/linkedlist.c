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

struct Node {
    Node *next;
    Node *prev;
    void *value;
};

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
    list->first = NULL;
    list->last = NULL;
    list->next = NULL;
    list->prev = NULL;
}

void* list_get(LinkedList *list, const size_t index)
{
    if (list == NULL)
        return NULL;

    size_t i;
    void *value = NULL;

    /* To make things a little faster, count from the beginning
     * if the index is in the first half of the list, and count
     * from the end if index is in the second half. */
    if (index < (list->size / 2)) {
        for (i = 0; i <= index; ++i)
            value = list_next(list);

        list->next = NULL;
    } else {
        for (i = list->size; i > index; --i)
            value = list_prev(list);

        list->prev = NULL;
    }

    return value;
}

void* list_next(LinkedList *list)
{
    if (list == NULL)
        return NULL;
    
    if (list->next == NULL)
        list->next = list->first;
    else
        list->next = list->next->next;

    return list->next == NULL ? NULL : list->next->value;
}

void* list_prev(LinkedList *list)
{
    if (list == NULL)
        return NULL;
    
    if (list->prev == NULL)
        list->prev = list->last;
    else
        list->prev = list->prev->prev;

    return list->prev == NULL ? NULL : list->prev->value;
}

void* list_append(LinkedList *list, void *value)
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
    return node->value;
}

void* list_prepop(LinkedList *list)
{
    if (list == NULL)
        return NULL;
    
    Node *nxtnode = list->first->next;
    nxtnode->prev = NULL;

    free(list->first);
    list->first = nxtnode;

    --list->size;
    return list->first->value;
}

size_t list_size(const LinkedList *list)
{
    return list->size;
}

void list_iter_reset(LinkedList *list)
{
    if (list == NULL)
        return;

    list->next = NULL;
    list->prev = NULL;
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

    list->first = NULL;
    list->last = NULL;
    list->next = NULL;
    list->prev = NULL;
    list->size = 0;
}
