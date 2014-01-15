#include <stdlib.h>
#include <string.h>
#include "xcurses.h"
#include "helper.h"
#include "message.h"
#include "interface.h"


void moveby(const int yinc, const int xinc)
{
    unsigned cury, curx;
    getyx(stdscr, cury, curx);  // Gets current cursor position.
    move(cury + yinc, curx + xinc);
}


void* safe_malloc(const size_t size, const MessageHistory *messages)
{
    void *newmem = malloc(size);
    if (!newmem)
        clean_exit(EXIT_FAILURE, messages);

    return newmem;
}


void clean_exit(const int status, const MessageHistory *messages)
{
    if (messages)
        /* Cast away const from messages. This is done so the other functions which
         * call clean_exit() can use a const version of messages. */
        hist_clear((MessageHistory *)messages);

    endwin();
    exit(status);
}


unsigned get_cursor(const int axis)
{
    unsigned cury, curx;
    getyx(stdscr, cury, curx);

    return (axis == Y) ? cury : curx;
}
