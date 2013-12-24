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
#include "network.h"

#define MAX_MSG_LEN 50
#define REFRESH_RATE 5
#define LOL 1


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
void echo_user_input(const char msgbuf[], const int screen_h, const int xstart);

// Handles all user input. Self-explanatory.
void handle_input(char msgbuf[]);

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

    draw_input_field(screen_w, screen_h);

    while(1)
    {
        // get_messages();
        // show_messages();
        echo_user_input(msgbuf, screen_h, 2);
        handle_input(msgbuf);
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

    for(int i = 0; i < length; ++i)
        addch('_');

    // Just add a 'prompt' character for stylistic reasons.
    printw("> ");
}


void echo_user_input(const char msgbuf[], const int screen_h, const int xstart)
{
    move(screen_h - 1, xstart);
    printw(msgbuf);
}


void handle_input(char msgbuf[])
{
    size_t msglen = strlen(msgbuf);
    int keyp = getch();

    if(keyp == ERR)
        return;  // No key pressed.

    // Enter key returns '\n'
    if(keyp == '\n')
    {
        // send_message(msgbuf);
        clear_input(msgbuf);
    }
    // There are multiple keys representing backspace.
    else if(keyp == KEY_BACKSPACE || keyp == 127 || keyp == 8)
    {
        backspace(msgbuf); 
    }
    else if(keyp >= ' ' && keyp <= '~')  // Printable character range.
    {
        if(msglen < MAX_MSG_LEN)
            append(msgbuf, keyp);
    }

    refresh();
}


void show_banner(void)
{
    printf("sChat v0.1\nUsage: ./schat [-flags] peer\n");
}
