#include <string.h>
#include "xcurses.h"
#include "txtfield.h"
#include "scrollpane.h"
#include "helper.h"

int sp_init(ScrollPane *sp, const unsigned x, const unsigned y, const size_t width, const size_t height)
{
    if (sp == NULL)
        return 0;

    sp->x = x;
    sp->y = y;
    sp->width = width;
    sp->height = height;

    sp->scroll_offset = 0;

    sp->win = newwin(height, width, y, x);
    if (sp->win == NULL)
        return 0;

    scrollok(sp->win, TRUE);  // Allows text to scroll in the window when hitting the bottom.

    return 1;
}

void sp_destroy(ScrollPane *sp)
{
    if (sp == NULL)
        return;

    delwin(sp->win);
    sp->win = NULL;
}

void sp_print(ScrollPane *sp, const char *txt)
{
    if (sp == NULL)
        return;

    sp_print_lines(sp, txt);
    wnoutrefresh(sp->win);  // Updates virtual screen but not physical screen.
}

void sp_print_lines(ScrollPane *sp, const char *txt)
{
    if (sp == NULL)
        return;

    /* Display at most sp->width - 1 characters of the line, and determine
     * which character in the line starts a newline. */
    const unsigned linec = (strlen(txt) / sp->width) + 1;
    for (unsigned line = 1; line <= linec; ++line)
        wprintw(sp->win, "%.*s", sp->width, txt + ((line - 1) * sp->width));
    waddch(sp->win, '\n');
}

void sp_scroll(ScrollPane *sp, const int dir)
{
    if (sp == NULL)
        return;

    if (dir < 0 && sp->scroll_offset > 0)
        --sp->scroll_offset;
    else if (dir > 0)
        ++sp->scroll_offset;
}

void sp_reset(ScrollPane *sp)
{
    if (sp == NULL)
        return;

    wclear(sp->win);
}
