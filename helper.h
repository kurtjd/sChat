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

// Performs cleanup then exits program.
void clean_exit(const int status, LinkedList *messages, ScrollPane *sp, TxtField *tf);

// Returns either the X or Y position of the cursor.
unsigned get_cursor(const unsigned axis);

#endif
