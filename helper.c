#include <ncurses.h>
#include <string.h>
#include "helper.h"

void append(char dest[], const char c)
{
    size_t destlen = strlen(dest);
    dest[destlen] = c;
    dest[destlen + 1] = '\0';
}


void moveby(const int yinc, const int xinc)
{
    int cury, curx;
    getyx(stdscr, cury, curx);  // Gets current cursor position.
    move(cury + yinc, curx + xinc);
}


void clear_input(char msgbuf[])
{
    // Moves the cursor to the beginning of the echo'd input.
    moveby(0, (strlen(msgbuf) * -1));

    // Clears all text from the cursor to the end of line.
    clrtoeol();

    msgbuf[0] = '\0';  // Clear the buffer.
}


void backspace(char msgbuf[])
{
    msgbuf[strlen(msgbuf) - 1] = '\0';

    // Moves the cursor to the left then deletes the character under it.
    moveby(0, -1);
    delch();
}
