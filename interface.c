#include <string.h>
#include "xcurses.h"
#include "helper.h"
#include "interface.h"


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
        // < as opposed to <= to account for NULL character.
        if(msglen < MAX_MSG_LEN)
            append(msgbuf, keyp);
    }

    refresh();
}
