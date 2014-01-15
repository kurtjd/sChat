#include <stdlib.h>
#include <stdio.h>
#include "xcurses.h"
#include "interface.h"
#include "txtfield.h"
#include "scrollpane.h"
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

    unsigned prev_msg_on = 0;  // Keeps track as the user cycles through sent messages.

    // Initialize the message history 'queue'.
    MessageHistory messages;
    hist_init(&messages, 250);  // Sets max history to 250 for now.

    // Initializes the window where all chat messages will appear.
    ScrollPane chatpane;
    sp_init(&chatpane, 0, 0, COLS, LINES - 1);

    // Initialize the sole text field to be used for input.
    TxtField input_field;
    tf_init(&input_field, 0, LINES - 1, COLS, MAX_MSG_LEN);
    
    while (1) {
        tf_draw(&input_field);
        handle_input(&messages, &chatpane, &input_field, &prev_msg_on);

        doupdate();  // Update all windows at once rather than individually.
    }

    clean_exit(EXIT_SUCCESS, &messages);  // Should never get to here...
}
