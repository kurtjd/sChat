#include <stdlib.h>
#include <string.h>
#include "xcurses.h"
#include "interface.h"
#include "txtfield.h"
#include "scrollpane.h"
#include "linkedlist.h"
#include "helper.h"

void moveby(const int yinc, const int xinc)
{
    unsigned cury, curx;
    getyx(stdscr, cury, curx);  // Gets current cursor position.
    move(cury + yinc, curx + xinc);
}

void clean_exit(const int status, LinkedList *messages, ScrollPane *sp, TxtField *tf)
{
    if (messages)
        list_clear(messages);
    if (sp)
        sp_destroy(sp);
    if (tf)
        tf_destroy(tf);

    endwin();
    exit(status);
}

unsigned get_cursor(const unsigned axis)
{
    unsigned cury, curx;
    getyx(stdscr, cury, curx);

    return (axis == Y) ? cury : curx;
}
