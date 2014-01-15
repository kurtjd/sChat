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

// Initialize linked list.
void list_init(LinkedList *list, const size_t maxsize);

// Adds a node to the linked list, then returns a pointer to it.
Node* list_append(LinkedList *list, void *value);

/* Frees the first node, then sets it to the second node.
 * Returns a pointer to the new beginning of the linked list. */
Node* list_pop_front(LinkedList *list);

// Frees all memory in the linked list.
void list_clear(LinkedList *list);

#endif
