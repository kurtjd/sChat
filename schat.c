#ifdef WIN32
#include <windows.h>
#include <curses.h>
#define XSLEEP(time) Sleep(time)
#else
#include <ncurses.h>
#define XSLEEP(time) nanosleep((struct timespec[]){{0, (time * 1000000L)}}, NULL)
#endif
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "helper.h"

#define MAX_MSG_LEN 50


/* Sets up the curses environment with preferred settings.
 * screen_h and screen_w are set by this function.
 * Remember: arguments are constant pointers not pointers to
 * constant data. The data WILL change, but the pointers
 * will never point to anything else! */
void init_curses(int * const screen_h, int * const screen_w);

/* Draws a horizontal line across the screen, separating the message pane from the input 'field'.
 * Pass the screen height because the y position should be screen_h - X where X is some offset. */
void draw_input_field(const int length, const int screen_h);

// Echo back what the user is currently typing.
void echo_user_input(const char msgbuf[]);

// Handles all user input. Self-explanatory.
void handle_input(char msgbuf[]);


int main(int argc, char *argv[])
{
    // These will be initialized by init_curses().
    int screen_h, screen_w;
    init_curses(&screen_h, &screen_w);

    /* As the user types, each character returned by getch() is stored here
     * until he presses enter. Then the message is sent and the buffer reset. */
    char msgbuf[MAX_MSG_LEN];

    while(1)
    {
        // get_messages();
        // show_messages();
        draw_input_field(screen_w, screen_h);
        echo_user_input(msgbuf);
        handle_input(msgbuf);
        refresh();
    }

    endwin();
    return 0;
}


void init_curses(int * const screen_h, int * const screen_w)
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


void draw_input_field(const int length, const int screen_h)
{
    // Sets the cursor position to the y offset.
    int offset = 2;
    move(screen_h - offset, 0);

    /* ACS_HLINE is a special character for a horizontal line,
     * with a normal dash '-' as the alternative character. */
    for(int i = 0; i < length; ++i) addch(ACS_HLINE);

    // Just add a 'prompt' character for stylistic reasons.
    printw("> ");
}


void echo_user_input(const char msgbuf[])
{
    printw(msgbuf);
}


void handle_input(char msgbuf[])
{
    size_t msglen = strlen(msgbuf);
    int keyp = getch();

    if(keyp == ERR) return;  // No key pressed.

    // Enter key returns '\n'
    if(keyp == '\n')
    {
        // send_message(msgbuf);
        clear_input(msgbuf);
    }
    else if(keyp == KEY_BACKSPACE)
    {
        backspace(msgbuf); 
    }
    else if(keyp >= ' ' && keyp <= '~')  // Printable character range.
    {
        if(msglen < MAX_MSG_LEN)
            append(msgbuf, keyp);
    }
}
