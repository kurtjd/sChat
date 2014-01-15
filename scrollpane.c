#include <string.h>
#include "scrollpane.h"
#include "xcurses.h"
#include "txtfield.h"
#include "helper.h"


void sp_init(ScrollPane *sp, const unsigned xpos, const unsigned ypos, const unsigned width, const unsigned height)
{
    sp->XPOS = xpos;
    sp->YPOS = ypos;
    sp->WIDTH = width;
    sp->HEIGHT = height;

    sp->scroll_offset = 0;

    sp->win = newwin(height, width, ypos, xpos);
    scrollok(sp->win, TRUE);  // Allows text to scroll in the window when hitting the bottom.
}

void sp_destroy(ScrollPane *sp)
{
    delwin(sp->win);
}

void sp_print(ScrollPane *sp, const char *txt)
{
    sp_print_lines(sp, txt);
    wprintw(sp->win, "\n");
    wnoutrefresh(sp->win);  // Updates virtual screen but not physical screen.
}

void sp_print_lines(ScrollPane *sp, const char *txt)
{
    /* Display at most sp->WIDTH - 1 characters of the line, and determine
     * which character in the line starts a newline. */
    unsigned linec = (strlen(txt) / sp->WIDTH) + 1;
    for (unsigned line = 1; line <= linec; ++line)
        wprintw(sp->win, "%.*s", sp->WIDTH, txt + ((line - 1) * sp->WIDTH));
}

void sp_scroll(ScrollPane *sp, const int dir)
{
    if (dir < 0 && sp->scroll_offset > 0)
        --sp->scroll_offset;
    else if (dir > 0)
        ++sp->scroll_offset;
}

void sp_reset(ScrollPane *sp)
{
    wclear(sp->win);
}
