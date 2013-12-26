#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "message.h"
#include "helper.h"

void history_init(MessageHistory *messages, const int max_history)
{
    if(!messages)
        clean_exit(EXIT_FAILURE, NULL);

    messages->max_history = max_history;
    messages->msg_count = 0;

    // Set these pointers to NULL so any attempt to dereference prematurely will crash.
    messages->first_msg = NULL;
    messages->last_msg = NULL;
}

void add_message(MessageHistory *messages, const int sender, const time_t timestamp, const char *msg)
{
    if(!messages)
        clean_exit(EXIT_FAILURE, NULL);

    Message *new_msg = new_message(messages, sender, timestamp, msg);

    if(!messages->first_msg)
    {
        messages->first_msg = new_msg;
    }
    else if(messages->msg_count >= messages->max_history)  // If at max history, gotta make room.
    {
        /* first_msg now points to what was the second message.
         * the original first_msg has been freed and deleted. */
        messages->first_msg = pop_front(messages);
    }

    // If this is the first message, we want next_msg to remain NULL.
    if(messages->msg_count > 0)
        messages->last_msg->next_msg = new_msg;

    messages->last_msg = new_msg;

    if(messages->msg_count < messages->max_history)
        ++messages->msg_count;
}


void clear_history(MessageHistory *messages)
{
    if(!messages)
        clean_exit(EXIT_FAILURE, NULL);

    Message *msg = messages->first_msg;
    while(msg)
    {
        Message *tmp = msg->next_msg;
        free(msg);
        msg = tmp;
    }
}


char* format_message(MessageHistory *messages, const int sender, const time_t timestamp, const char *msg)
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


/* BELOW FUNCTIONS ARE INTERNAL USE ONLY. SHOULD NOT BE USED OUTSIDE THIS LIBRARY. */

Message* new_message(MessageHistory *messages, const int sender, const time_t timestamp, const char *msg)
{
    Message *new_msg = safe_malloc(sizeof(Message), messages);
    new_msg->next_msg = NULL;
    new_msg->sender = sender;
    new_msg->timestamp = timestamp;

    // Can't use sizeof on an array passed into a function, so add 1 to account for null char.
    strncpy(new_msg->txt, msg, strlen(msg) + 1);

    return new_msg;
}

Message* pop_front(MessageHistory *messages)
{
    if(!messages)
        clean_exit(EXIT_FAILURE, NULL);
    
    Message *nxtmsg = messages->first_msg->next_msg;
    free(messages->first_msg);
    return nxtmsg;
}
