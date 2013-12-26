#include <stdlib.h>
#include <string.h>
#include "xcurses.h"
#include "helper.h"
#include "interface.h"
#include "message.h"


void init_curses(int *screen_h, int *screen_w)
{
    /* I know it is 'good practice' to always check for null pointers
     * in functions that accept arguments passed by address, but
     * seeing as this isn't some API I'd rather not clutter code
     * with all that checking, especially in cases where you'd have to
     * intentionally pass a null pointer such as this. */
    initscr();
    cbreak();
    noecho();  // Gotta turn @echo off!
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);  // Makes getch() non-blocking.

    /* Dereference here because getmaxyx(), like many ncurses functions,
     * is actually a macro. This essentially expands to:
     *
     * *screen_h = ROWS;
     * *screen_w = COLS;   */
    getmaxyx(stdscr, *screen_h, *screen_w);
}


void show_message_history(MessageHistory *messages, const int screen_h, const int screen_w)
{
    /* This is done so the message history isn't redrawn every cycle.
     * If prev_last_msg doesn't hold the same address as the current last_msg,
     * we can conclude a new message was added. */
    static Message *prev_last_msg = NULL;
    if(prev_last_msg == messages->last_msg)
        return;

    move(0, 0);  // Position cursor

    // Loop through each message by following the chain until we hit a null pointer.
    // Obviously display will be made prettier at a later time.
    Message *msg = messages->first_msg;
    while(msg)
    {
        char *final_msg = format_message(messages, msg->sender, msg->timestamp, msg->txt);
        printw("%s\n", final_msg);
        free(final_msg);

        msg = msg->next_msg;
    }

    prev_last_msg = messages->last_msg;
}


void draw_input_field(const int length, const int screen_h)
{
    // Sets the cursor position to the y offset.
    const int OFFSET = 2;
    move(screen_h - OFFSET, 0);

    for(int i = 0; i < length; ++i)
        addch('_');

    // Just add a 'prompt' character for stylistic reasons.
    printw("> ");
}


void echo_user_input(const char *msgbuf, const int screen_h, const int xstart)
{
    move(screen_h - 1, xstart);
    printw(msgbuf);
}


void handle_input(char *msgbuf, MessageHistory *messages)
{
    size_t msglen = strlen(msgbuf);
    int keyp = getch();

    if(keyp == ERR)
        return;  // No key pressed.

    // Enter key returns '\n', and don't send blank messages.
    if(keyp == '\n' && msglen > 0)
    {
        // send_message(msgbuf);
        add_message(messages, FROM_SELF, time(0), msgbuf);
        clear_input(msgbuf);
    }
    // There are multiple keys representing backspace.
    else if(keyp == KEY_BACKSPACE || keyp == 127 || keyp == 8)
    {
        backspace(msgbuf); 
    }
    else if(keyp >= ' ' && keyp <= '~')  // Printable character range.
    {
        // < as opposed to <= to account for NULL character.
        if(msglen < MAX_MSG_LEN)
            append(msgbuf, keyp);
    }
}
