/* Just a collection of miscellaneous helper functions that don't need to
 * be cluttering the main source file. */
#ifndef HELPER_H
#define HELPER_H

#include <string.h>
#include "message.h"


// Moves the cursor by specified amount.
void moveby(const int yinc, const int xinc);

// Checks if malloc was successful and peforms cleanup if not.
void* safe_malloc(const size_t size, const MessageHistory *messages);

// Performs cleanup then exits program.
void clean_exit(const int status, const MessageHistory *messages);

#endif
