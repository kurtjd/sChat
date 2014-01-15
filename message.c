#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "message.h"
#include "scrollpane.h"
#include "helper.h"

// Creates a new instance of the Message struct on the heap.
static Message* msg_new(MessageHistory *messages, const unsigned sender, const time_t timestamp, const char *msg)
{
    Message *new_msg = safe_malloc(sizeof(Message), messages);
    new_msg->next_msg = NULL;

    // If first message, this will be null as intended.
    new_msg->prev_msg = messages->last_msg;

    new_msg->sender = sender;
    new_msg->timestamp = timestamp;

    // Can't use sizeof on an array passed into a function, so add 1 to account for null char.
    strncpy(new_msg->txt, msg, strlen(msg) + 1);

    return new_msg;
}

// Combines all the elements of a message into a string.
static char* msg_format(const MessageHistory *messages, const unsigned sender, const time_t timestamp, const char *msg)
{
    (void)timestamp;  // SUPPRESS UNUSED PARAMETER WARNING ONLY FOR NOW!

    // This will need to be dynamically allocated once I use names instead of the below defaults.
    char msg_sender[7];
    strncpy(msg_sender, (sender == FROM_SELF) ? "You:  " : "Peer: ", 7);

    /* Allocate enough space to hold the name along with the message.
    * +1 to account for null character. */
    char *message = safe_malloc(strlen(msg_sender) + strlen(msg) + 1, messages);

    // Then copy individual strings into message.
    strncpy(message, msg_sender, sizeof msg_sender);
    strncat(message, msg, strlen(msg) + 1);  // +1 for null char

    return message;
}

/* When a new message is added and the history limit is at its max, delete the oldest message.
 * Also returns the 2nd message to be set as the new first message. */
static Message* hist_pop_front(MessageHistory *messages)
{
    if (messages == NULL)
        return NULL;
    
    Message *nxtmsg = messages->first_msg->next_msg;
    nxtmsg->prev_msg = NULL;
    free(messages->first_msg);
    return nxtmsg;
}


void hist_init(MessageHistory *messages, const unsigned max_history)
{
    if (messages == NULL)
        return;

    messages->max_history = max_history;
    messages->msg_count = 0;

    // Set these pointers to NULL so any attempt to dereference prematurely will crash.
    messages->first_msg = NULL;
    messages->last_msg = NULL;
}

Message* hist_add_message(MessageHistory *messages, const unsigned sender, const time_t timestamp, const char *msg)
{
    if (messages == NULL)
        return NULL;

    Message *new_msg = msg_new(messages, sender, timestamp, msg);

    if (!messages->first_msg)
        messages->first_msg = new_msg;
    else if (messages->msg_count >= messages->max_history)
        /* If at max history, gotta make room.
         * first_msg now points to what was the second message.
         * the original first_msg has been freed and deleted. */
        messages->first_msg = hist_pop_front(messages);

    // If this is the first message, we want next_msg to remain NULL.
    if (messages->msg_count > 0)
        messages->last_msg->next_msg = new_msg;

    messages->last_msg = new_msg;

    if (messages->msg_count < messages->max_history)
        ++messages->msg_count;

    return new_msg;
}

void hist_clear(MessageHistory *messages)
{
    if (messages == NULL)
        return;

    Message *msg = messages->first_msg;
    while (msg) {
        Message *tmp = msg->next_msg;
        free(msg);
        msg = tmp;
    }
}

void hist_print_messages(const MessageHistory *messages, ScrollPane *sp)
{
    if (messages == NULL)
        return;

    sp_reset(sp);
    
    for (Message *msg = messages->first_msg; msg != NULL; msg = msg->next_msg)
        msg_print(messages, msg, sp);
}

int hist_get_linecount(const MessageHistory *messages, const size_t screenw)
{
    if (messages == NULL)
        return -1;

    int total_lines = 0;

    for (Message *msg = messages->first_msg; msg != NULL; msg = msg->next_msg) {
        char *final_msg = msg_format(messages, msg->sender, msg->timestamp, msg->txt);
        total_lines += ((strlen(final_msg) / screenw) + 1);
        free(final_msg);
    }

    return total_lines;
}

void msg_print(const MessageHistory *messages, const Message *message, ScrollPane *sp)
{
    char *msg = msg_format(messages, message->sender, message->timestamp, message->txt);
    sp_print(sp, msg);
    free(msg);
}
