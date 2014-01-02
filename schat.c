#include <stdlib.h>
#include <stdio.h>
#include "xcurses.h"
#include "interface.h"
#include "network.h"
#include "message.h"
#include "helper.h"


// Show usage banner
void show_banner(void);


int main(int argc, char *argv[])
{
    // SUPRRESSES GCC UNUSED VARIABLE WARNING!
    // THIS IS ONLY TEMPORARY!
    (void)argc;
    (void)argv;
 
    int screen_h, screen_w;
    init_curses(&screen_h, &screen_w);

    /* As the user types, each character returned by getch() is stored here
     * until he presses enter. Then the message is sent and the buffer reset. */
    char msgbuf[MAX_MSG_LEN] = "";
    int echo_start = 0;  // Which character in the user input to start echoing.

    // Create and initialize the message history 'queue'.
    MessageHistory messages;
    history_init(&messages, 250);  // Sets max history to 250 for now.
    
    while(1)
    {
        draw_input_field(screen_w, screen_h);
        show_message_history(&messages, screen_h, screen_w);
        echo_user_input(msgbuf, screen_h, echo_start);
        handle_input(msgbuf, &messages, &screen_h, &screen_w, &echo_start);

        /* This only updates part of the screen to change,
         * and since I'm careful to only change when necessary,
         * this is efficient. */
        refresh();
    }

    clean_exit(EXIT_SUCCESS, &messages);  // Should never get to here...
}


void show_banner(void)
{
    printf("sChat v0.1\nUsage: ./schat [-flags] peer\n");
}
