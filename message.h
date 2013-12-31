/* Message history is stored in a queue which is implemented as a linked-list.
 * This header also contains functions that work with the messages. */
#ifndef MESSAGE_H
#define MESSAGE_H

#include <time.h>

/* Holy shit really strange bug where if this is 300 or 500 exactly, some very strange output
 * is spat out if the user attempts to type more than the max length and the program aborts.
 * This isn't a segmentation fault because I check the user isn't trying to type any more
 * than the max length. This problem ONLY happens when set to 300 or 500. (AFAIK) */
#define MAX_MSG_LEN 400

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
    int sender;  // Either FROM_SELF or FROM_PEER.
    time_t timestamp;
    char txt[MAX_MSG_LEN];
};

typedef struct
{
    int max_history;
    int msg_count;  // A count of messages aka size of history.
    Message *first_msg;
    Message *last_msg;
} MessageHistory;


// Initializes the message history.
void history_init(MessageHistory *messages, const int max_history);

// Adds a message to the message history to be displayed.
void add_message(MessageHistory *messages, const int sender, const time_t timestamp, const char *msg);

// Frees all memory in the linked list.
void clear_history(MessageHistory *messages);

// Returns a formatted message ready to be printed.
char* format_message(const MessageHistory *messages, const int sender, const time_t timestamp, const char *msg);


/* BELOW FUNCTIONS ARE INTERNAL USE ONLY. SHOULD NOT BE USED OUTSIDE THIS LIBRARY. */

// Creates a new instance of the Message struct on the heap.
Message* new_message(MessageHistory *messages, const int sender, const time_t timestamp, const char *msg);

/* When a new message is added and the history limit is at its max, delete the oldest message.
 * Also returns the 2nd message to be set as the new first message. */
Message* pop_front(MessageHistory *messages);

#endif
