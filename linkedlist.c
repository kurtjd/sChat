#include <stdlib.h>
#include "linkedlist.h"

// Creates a new node.
static Node* list_new_node(LinkedList *list, void *value)
{
    Node *node = malloc(sizeof *node);
    if(node == NULL)
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

    list->maxsize = maxsize;
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

    if (list->first == NULL)
        list->first = node;
    else if (list->size >= list->maxsize)  // If at max size, make room for new node.
        list_pop_front(list);

    // If this is the first node, we want next to remain NULL.
    if (list->size > 0)
        list->last->next = node;

    list->last = node;

    ++list->size;
    return node;
}

Node* list_pop_front(LinkedList *list)
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
        free(node);
        node = tmp;
    }

    list->size = 0;
}
