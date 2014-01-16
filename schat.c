#include <stdlib.h>
#include <stdio.h>
#include "xcurses.h"
#include "interface.h"
#include "txtfield.h"
#include "scrollpane.h"
#include "network.h"
#include "message.h"
#include "linkedlist.h"
#include "helper.h"

// Show usage banner
void show_banner(void)
{
    printf("sChat v0.1\nUsage: ./schat [-flags] peer\n");
}

int main()
{
    init_curses();

    unsigned prev_msg_on = 0;  // Keeps track as the user cycles through sent messages.

    // Initialize the message history list.
    LinkedList messages;
    list_init(&messages, 250);  // Sets max history to 250 for now.

    // Initializes the window where all chat messages will appear.
    ScrollPane chatpane;
    if(!sp_init(&chatpane, 0, 0, COLS, LINES - 1))
        clean_exit(EXIT_FAILURE, NULL, NULL, NULL);

    // Initialize the sole text field to be used for input.
    TxtField input_field;
    if(!tf_init(&input_field, 0, LINES - 1, COLS, MAX_MSG_LEN))
        clean_exit(EXIT_FAILURE, NULL, NULL, NULL);
    
    while (1) {
        tf_draw(&input_field);

        if(!handle_input(&messages, &chatpane, &input_field, &prev_msg_on))
            clean_exit(EXIT_FAILURE, &messages, &chatpane, &input_field);

        doupdate();  // Update all windows at once rather than individually.
    }

    clean_exit(EXIT_SUCCESS, &messages, &chatpane, &input_field);  // Should never get to here...
}
