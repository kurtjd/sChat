#include <stdlib.h>
#include <stdio.h>
#include "xcurses.h"
#include "interface.h"
#include "txtfield.h"
#include "network.h"
#include "message.h"
#include "helper.h"


// Show usage banner
void show_banner(void)
{
    printf("sChat v0.1\nUsage: ./schat [-flags] peer\n");
}

int main(int argc, char *argv[])
{
    // SUPRRESSES GCC UNUSED VARIABLE WARNING!
    // THIS IS ONLY TEMPORARY!
    (void)argc;
    (void)argv;
 
    init_curses();

    int hist_start = 0;  // Where in the history to begin showing messages.
    int prev_msg_on = 0;  // Keeps track as the user cycles through sent messages.

    // Create and initialize the message history 'queue'.
    MessageHistory messages;
    history_init(&messages, 250);  // Sets max history to 250 for now.

    // Create and initialize the single text field to be used for input.
    TxtField txtbox;
    tf_init(&txtbox, 0, LINES - 1, COLS, MAX_MSG_LEN);
    
    while(1)
    {
        tf_draw(&txtbox);
        show_message_history(&messages, &hist_start);
        tf_echo(&txtbox);
        handle_input(&txtbox, &messages, &hist_start, &prev_msg_on);

        /* This only updates part of the screen to change,
         * and since I'm careful to only change when necessary,
         * this is efficient. */
        refresh();
    }

    clean_exit(EXIT_SUCCESS, &messages);  // Should never get to here...
}
