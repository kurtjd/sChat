#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "message.h"

void history_init(MessageHistory *messages, const int max_history)
{
    messages->max_history = max_history;
    messages->msg_count = 0;

    // Set these pointers to 0 so any attempt to dereference prematurely will crash.
    messages->first_msg = 0;
    messages->last_msg = 0;
}

void add_message(MessageHistory *messages, const int sender, const time_t timestamp, const char msg[])
{
    Message *new_msg = new_message(sender, timestamp, msg);

    if(!messages->first_msg)
    {
        // Create the first link in the chain.
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
    Message *msg = messages->first_msg;
    while(msg)
    {
        Message *tmp = msg->next_msg;
        free(msg);
        msg = tmp;
    }
}


/* BELOW FUNCTIONS ARE INTERNAL USE ONLY. SHOULD NOT BE USED OUTSIDE THIS LIBRARY. */

Message* new_message(const int sender, const time_t timestamp, const char msg[])
{
    Message *new_msg = (Message *)malloc(sizeof(Message));
    new_msg->next_msg = 0;
    new_msg->sender = sender;
    new_msg->timestamp = timestamp;
    strcpy(new_msg->txt, msg);

    return new_msg;
}

Message* pop_front(MessageHistory *messages)
{
    Message *nxtmsg = messages->first_msg->next_msg;
    free(messages->first_msg);
    return nxtmsg;
}
