/* Message history is stored in a queue which is implemented as a linked-list.
 * This header also contains functions that work with the messages. */
#ifndef MESSAGE_H
#define MESSAGE_H

#include <time.h>
#include <string.h>
#include "scrollpane.h"

#define MAX_MSG_LEN 500

#define FROM_SELF 0
#define FROM_PEER 1


/* The components of a message.
 * And yes, there is a reason behind this madness.
 * If I don't declare the struct before typedef'ing it, I have to use struct Message
 * within the body because the typedef isn't valid yet. This wouldn't be much of a problem,
 * however GCC gives me warnings for using the typedef Message as opposed to struct Message
 * when dealing with next_msg. I don't like warnings. */
struct Message;
typedef struct Message Message;
struct Message
{
    Message *next_msg;
    Message *prev_msg;
    unsigned sender;  // Either FROM_SELF or FROM_PEER.
    time_t timestamp;
    char txt[MAX_MSG_LEN];
};

typedef struct
{
    unsigned max_history;
    unsigned msg_count;  // A count of messages aka size of history.
    Message *first_msg;
    Message *last_msg;
} MessageHistory;


// Initializes the message history.
void hist_init(MessageHistory *messages, const unsigned max_history);

// Adds a message to the message history to be displayed, then returns a pointer to it.
Message* hist_add_message(MessageHistory *messages, const unsigned sender, const time_t timestamp, const char *msg);

// Frees all memory in the linked list.
void hist_clear(MessageHistory *messages);

// Prints all messages to the chat pane.
void hist_print_messages(const MessageHistory *messages, ScrollPane *sp);

// Returns the total number of lines of all messages in the history.
int hist_get_linecount(const MessageHistory *messages, const size_t screenw);

// Prints a single message to the chatpane.
void msg_print(const MessageHistory *messages, const Message *message, ScrollPane *sp);

#endif
