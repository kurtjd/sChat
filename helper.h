#ifndef HELPER_H
#define HELPER_H

#include <stdlib.h>
#include "txtfield.h"
#include "scrollpane.h"
#include "linkedlist.h"

#define X 0
#define Y 1

// Moves the cursor by specified amount.
void moveby(const int yinc, const int xinc);

// Checks if malloc was successful and peforms cleanup if not.
void* safe_malloc(const size_t size);

// Performs cleanup then exits program.
void clean_exit(const int status, LinkedList *messages, ScrollPane *sp, TxtField *tf);

// Returns either the X or Y position of the cursor.
unsigned get_cursor(const unsigned axis);

#endif
