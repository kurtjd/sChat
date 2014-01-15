#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "message.h"
#include "linkedlist.h"
#include "scrollpane.h"
#include "helper.h"

// Combines all the elements of a message into a string. Only used internally hence the static qualifier.
static char* msg_format(const unsigned sender, const time_t timestamp, const char *msg)
{
    (void)timestamp;  // SUPPRESS UNUSED PARAMETER WARNING ONLY FOR NOW!

    // This will need to be dynamically allocated once I use names instead of the below defaults.
    char msg_sender[7];
    strncpy(msg_sender, (sender == FROM_SELF) ? "You:  " : "Peer: ", 7);

    /* Allocate enough space to hold the name along with the message.
    * +1 to account for null character. */
    char *message = calloc(strlen(msg_sender) + strlen(msg) + 1, sizeof(char));

    // Then copy individual strings into message.
    strncpy(message, msg_sender, sizeof msg_sender);
    strncat(message, msg, strlen(msg) + 1);  // +1 for null char

    return message;
}

Message* msg_new(LinkedList *messages, const unsigned sender, const time_t timestamp, const char *msg)
{
    Message *new_msg = malloc(sizeof *new_msg);

    new_msg->sender = sender;
    new_msg->timestamp = timestamp;

    // Can't use sizeof on an array passed into a function, so add 1 to account for null char.
    strncpy(new_msg->txt, msg, strlen(msg) + 1);

    list_append(messages, new_msg);

    return new_msg;
}

void msg_print(const Message *message, ScrollPane *sp)
{
    char *msg = msg_format(message->sender, message->timestamp, message->txt);
    sp_print(sp, msg);
    free(msg);
    msg = NULL;
}

// The below two functions will be going away once ScrollPane is complete.
void msg_print_all(const LinkedList *messages, ScrollPane *sp)
{
    if (messages == NULL)
        return;

    sp_reset(sp);
    
    for (Node *node = messages->first; node != NULL; node = node->next)
        msg_print(node->value, sp);
}

int msg_all_linec(const LinkedList *messages, const size_t screenw)
{
    if (messages == NULL)
        return -1;

    int total_lines = 0;

    for (Node *node = messages->first; node != NULL; node = node->next) {
        const Message *msg = node->value;
        char *final_msg = msg_format(msg->sender, msg->timestamp, msg->txt);

        total_lines += ((strlen(final_msg) / screenw) + 1);
        free(final_msg);
        final_msg = NULL;
    }

    return total_lines;
}
