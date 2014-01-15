#include <stdlib.h>
#include <string.h>
#include "xcurses.h"
#include "interface.h"
#include "linkedlist.h"
#include "helper.h"

void moveby(const int yinc, const int xinc)
{
    unsigned cury, curx;
    getyx(stdscr, cury, curx);  // Gets current cursor position.
    move(cury + yinc, curx + xinc);
}

void* safe_malloc(const size_t size, const LinkedList *messages)
{
    void *newmem = malloc(size);
    if (!newmem)
        clean_exit(EXIT_FAILURE, messages);

    return newmem;
}

void clean_exit(const int status, const LinkedList *messages)
{
    /* Cast away const from messages. This is done so the other functions which
     * call clean_exit() can use a const version of messages. */
    if (messages)
        list_clear((LinkedList *)messages);

    endwin();
    exit(status);
}

unsigned get_cursor(const unsigned axis)
{
    unsigned cury, curx;
    getyx(stdscr, cury, curx);

    return (axis == Y) ? cury : curx;
}
