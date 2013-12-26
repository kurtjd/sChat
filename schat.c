#include <stdlib.h>
#include <stdio.h>
#include "xcurses.h"
#include "interface.h"
#include "network.h"
#include "message.h"
#include "helper.h"

#define LOL 1


// Show usage banner
void show_banner(void);


int main()  // Removed argc and argv for now to avoid warnings.
{
    // These will be initialized by init_curses().
    int screen_h, screen_w;
    init_curses(&screen_h, &screen_w);

    /* As the user types, each character returned by getch() is stored here
     * until he presses enter. Then the message is sent and the buffer reset. */
    char msgbuf[MAX_MSG_LEN] = "";

    // Create and initialize the message history 'queue'.
    MessageHistory messages;
    history_init(&messages, 10);  // Sets max history to 10 for now.
    
    draw_input_field(screen_w, screen_h);

    while(1)
    {
        show_message_history(&messages, screen_h, screen_w);
        echo_user_input(msgbuf, screen_h, 2);
        handle_input(msgbuf, &messages);

        /* This only updates part of the screen to change,
         * and since I'm careful to only change when necessary,
         * this is efficient. */
        refresh();
    }

    clean_exit(EXIT_SUCCESS, &messages);
    return 0;
}


void show_banner(void)
{
    printf("sChat v0.1\nUsage: ./schat [-flags] peer\n");
}
