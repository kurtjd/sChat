/* Message history is stored in a queue which is implemented as a linked-list.
 * This header also contains functions that work with the messages. */
#ifndef MESSAGE_H
#define MESSAGE_H

#include <time.h>

#define MAX_MSG_LEN 51
#define MAX_HISTORY 100


// The components of a message.
typedef struct
{
    int sender;  // Either 1 for other person or 0 for sent from self.
    time_t timestamp;
    char txt[MAX_MSG_LEN];
} Message;


// Adds a message to the message history to be displayed.
void add_message(const int sender, const time_t timestamp, const char msg[], Message messages[]);

#endif
