#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "xcurses.h"
#include "helper.h"
#include "interface.h"
#include "message.h"


void init_curses(int *screen_h, int *screen_w)
{
    if(screen_h == NULL || screen_w == NULL)
        clean_exit(EXIT_FAILURE, NULL);

    initscr();
    cbreak();
    noecho();  // Gotta turn @echo off!
    keypad(stdscr, 1);
    nodelay(stdscr, 1);  // Makes getch() non-blocking.

    /* Dereference here because getmaxyx(), like many ncurses functions,
     * is actually a macro. This essentially expands to:
     *
     * *screen_h = ROWS;
     * *screen_w = COLS;   */
    getmaxyx(stdscr, *screen_h, *screen_w);
}


void show_message_history(const MessageHistory *messages, const int screen_h, const int screen_w)
{
    if(messages == NULL)
        clean_exit(EXIT_FAILURE, NULL);

    /* This is done so the message history isn't redrawn every cycle.
     * If prev_last_msg doesn't hold the same address as the current last_msg,
     * we can conclude a new message was added. */
    static Message *prev_last_msg = NULL;
    if(prev_last_msg == messages->last_msg)
        return;

    static int screenfull = 0;  // Is the screen full of messages?

    int maxlines = screen_h - (INPUT_HEIGHT + 1);
    int starty = screenfull ? maxlines : 0;

    move(0, 0);  // Position cursor
    
    /* Loop through each message by following the chain until we hit a null pointer.
     * Whether or not the screen is full determines if we go backwards or forwards. */
    Message *msg = screenfull ? messages->last_msg : messages->first_msg;
    while(msg)
    {
        char *final_msg = format_message(messages, msg->sender, msg->timestamp, msg->txt);
        int msglines = (strlen(final_msg) / screen_w) + 1;

        if(screenfull)
            print_lines(final_msg, msglines, screen_w, &starty);
        else
        {
            print_message(final_msg, msglines, maxlines, &screenfull, &starty, messages, screen_h, screen_w);
            if(screenfull)
                return;
        }

        free(final_msg);
        msg = screenfull ? msg->prev_msg : msg->next_msg;
    }

    prev_last_msg = messages->last_msg;

}


void draw_input_field(const int length, const int screen_h)
{
    // Sets the cursor position to the y offset.
    move(screen_h - INPUT_HEIGHT, 0);

    for(int i = 0; i < length; ++i)
        addch('_');

    // Just add a 'prompt' character for stylistic reasons.
    printw("> ");
}


void echo_user_input(const char *msgbuf, const int screen_h, const int screen_w, const int xstart)
{
    move(screen_h - 1, xstart);

    // Only print characters that fit on the screen.
    printw("%.*s", screen_w - PROMPT_LEN, msgbuf);
}


void handle_input(char *msgbuf, MessageHistory *messages, const unsigned screen_w)
{
    if(messages == NULL)
        clean_exit(EXIT_FAILURE, NULL);

    size_t msglen = strlen(msgbuf);
    int keyp = getch();

    if(keyp == ERR)
        return;  // No key pressed.

    // Enter key returns '\n', and don't send blank messages.
    if(keyp == '\n' && msglen > 0)
    {
        add_message(messages, FROM_SELF, time(0), msgbuf);
        clear_input(msgbuf, screen_w);
    }

    // There are multiple keys representing backspace.
    else if(keyp == KEY_BACKSPACE || keyp == 127 || keyp == 8)
    {
        backspace(msgbuf); 
    }
    else if(isprint(keyp))
    {
        // < as opposed to <= to account for NULL character.
        if(msglen < MAX_MSG_LEN)
            append(msgbuf, keyp);
    }
}


void print_lines(const char *msg, const int msglines, const int screen_w, int *starty)
{
    /* Display each line from the bottom-up.
     * starty is checked because attempting to move the cursor
     * beyond the screen and then drawing causes problems. */
    for(int line = msglines; line > 0 && *starty >= 0; --line)
    {
        // Position cursor and clear all previous text on the line.
        move((*starty)--, 0);
        clrtoeol();

        /* Display at most screen_w characters of the line, and determine
         * which character in the line starts a newline. */
        printw("%.*s", screen_w, msg + ((line - 1) * screen_w));
    }
}


void print_message(char *msg, const int msglines, const int maxlines, int *screenfull,
                   int *starty, const MessageHistory *messages, const int screen_h, const int screen_w)
{
    *starty += msglines;

    if(*starty > maxlines)
    {
        *screenfull = 1;
        free(msg);

        // Make a recursive call to the parent function  in order to repeat it as if the screen were full.
        show_message_history(messages, screen_h, screen_w);
        return;
    }

    // Lines break automatically when printing downward, so no need for anything fancy.
    printw("%s\n", msg);
}
