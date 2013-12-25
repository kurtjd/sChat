#include <stdio.h>
#include "xcurses.h"
#include "interface.h"
#include "network.h"
#include "message.h"

#define LOL 1


// Show usage banner
void show_banner(void);


int main(int argc, char *argv[])
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
        // get_messages();
        show_message_history(&messages);
        echo_user_input(msgbuf, screen_h, 2);
        handle_input(msgbuf, &messages);
    }

    clear_history(&messages);
    endwin();
    return 0;
}


void show_banner(void)
{
    printf("sChat v0.1\nUsage: ./schat [-flags] peer\n");
}
