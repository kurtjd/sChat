#include <string.h>
#include "xcurses.h"
#include "txtfield.h"
#include "scrollpane.h"
#include "linkedlist.h"

// Given a position in a string, creates a new string that would fit on one line.
static char* sp_new_line(ScrollPane *sp, const char *txt, const size_t lstart)
{
    // Determines where in the string a new line would start, and now long this line should be.
    const char *line_start = txt + (lstart * sp->width);
    const size_t linelen = strlen(line_start) >= sp->width ? sp->width : strlen(line_start);

    char *line = malloc(linelen + 1);
    if (line == NULL)
        return NULL;

    strncpy(line, line_start, linelen);
    line[linelen] = '\0';

    if (list_append(&sp->lines, line) == NULL)
        return NULL;

    return line;
}

int sp_init(ScrollPane *sp, const unsigned x, const unsigned y, const size_t width, const size_t height)
{
    if (sp == NULL)
        return 0;

    sp->x = x;
    sp->y = y;
    sp->width = width;
    sp->height = height;

    list_init(&sp->lines, 0);
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
    list_clear(&sp->lines);
}

int sp_print(ScrollPane *sp, const char *txt)
{
    if (sp == NULL)
        return 0;
    
    // Breaks the string into lines and appends each line to the linked-list, then prints it.
    const unsigned linec = (strlen(txt) / sp->width) + 1;
    for (unsigned i = 0; i < linec; ++i) {
        char *line = sp_new_line(sp, txt, i);
        if(line == NULL)
            return 0;

        wprintw(sp->win, "%s", line);
    }

    waddch(sp->win, '\n');
    wnoutrefresh(sp->win);  // Updates virtual screen but not physical screen.

    return 1;
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
