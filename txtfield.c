#include <stdlib.h>
#include <string.h>
#include "txtfield.h"
#include "xcurses.h"
#include "helper.h"

void tf_init(TxtField *txtbox, const unsigned xpos, const unsigned ypos, const size_t width, const size_t maxlen)
{
    txtbox->XPOS = xpos;
    txtbox->YPOS = ypos;
    txtbox->WIDTH = width;
    txtbox->HEIGHT = 1;

    txtbox->MAXLEN = maxlen;
    txtbox->SCROLL_GAP = 20;

    txtbox->contents = malloc(maxlen + 1);  // MAKE SURE TO FREE ON EXIT!
    txtbox->length = 0;

    txtbox->echo_start = 0;
    txtbox->cursor_offset = 0;
}

void tf_scale(TxtField *txtbox, const unsigned newx, const unsigned newy, const unsigned newwidth)
{
    txtbox->XPOS = newx;
    txtbox->YPOS = newy;
    txtbox->WIDTH = newwidth;
}

void tf_clear(TxtField *txtbox)
{
    // Moves the cursor to the beginning of the echo'd input.
    move(txtbox->YPOS, txtbox->XPOS);

    // Clears all text from the cursor to the end of line.
    clrtoeol();

    txtbox->echo_start = 0;
    txtbox->cursor_offset = 0;

    txtbox->contents[0] = '\0';  // Clear the buffer.
    txtbox->length = 0;
}

void tf_backspace(TxtField *txtbox)
{
    // Don't want this function running if there is no input.
    if(txtbox->length < 1)
        return;

    // The index in the message buffer to perform deletion.
    unsigned insert_at = ((get_cursor(X) - txtbox->XPOS) - 1) + (txtbox->SCROLL_GAP * txtbox->echo_start);

    for(size_t i = insert_at; i < txtbox->length; ++i)
        txtbox->contents[i] = txtbox->contents[i + 1];

    // Moves the cursor to the left then deletes the character under it.
    moveby(0, -1);
    delch();
    --txtbox->length;

    if(get_cursor(X) <= txtbox->XPOS)
        tf_reset_echo(txtbox);
        /* Remember: This can cause prompt to be removed by tf_backspace if cursor offset is not 0.
         * Will fix after fixing the issue with inserting text in the middle of the message. */
}

void tf_insert(TxtField *txtbox, const char c)
{
    if(txtbox->length >= (txtbox->MAXLEN - 1))
        return;

    // The index in the message buffer c is to be inserted.
    unsigned insert_at = (get_cursor(X) - txtbox->XPOS) + (txtbox->SCROLL_GAP * txtbox->echo_start);

    // Move characters to the right that are after the insertion point.
    for(size_t i = txtbox->length; i > insert_at; --i)
        txtbox->contents[i] = txtbox->contents[i - 1];

    txtbox->contents[insert_at] = c;
    txtbox->contents[txtbox->length + 1] = '\0';
    ++txtbox->length;

    if(get_cursor(X) == (txtbox->XPOS + txtbox->WIDTH - 1))
        tf_reset_echo(txtbox);
}

void tf_set(TxtField *txtbox, const char *str)
{
    strncpy(txtbox->contents, str, strlen(str) + 1);
    txtbox->length = strlen(str);
}

void tf_echo(TxtField *txtbox)
{
    move(txtbox->YPOS, txtbox->XPOS);
    printw("%s", txtbox->contents + (txtbox->SCROLL_GAP * txtbox->echo_start));

    // Moves the cursor back to it's offset position if the user has moved it.
    tf_move_cursor(txtbox, 0);
}

void tf_reset_echo(TxtField *txtbox)
{
    move(txtbox->YPOS, txtbox->XPOS);
    clrtoeol();
    txtbox->echo_start = 0;

    if(txtbox->length >= txtbox->WIDTH)
        txtbox->echo_start = 1 + ((txtbox->length - (txtbox->WIDTH)) / txtbox->SCROLL_GAP);
}

void tf_move_cursor(TxtField *txtbox, const int dir)
{
    unsigned cursy, cursx;
    getyx(stdscr, cursy, cursx);

    if(dir < 0 && cursx > txtbox->XPOS)
        --txtbox->cursor_offset;
    else if(dir > 0 && cursx < (txtbox->XPOS + txtbox->WIDTH) - (txtbox->SCROLL_GAP * txtbox->echo_start))
        ++txtbox->cursor_offset;

    move(cursy, cursx + txtbox->cursor_offset);
}

void tf_draw(const TxtField *txtbox)
{
    // Sets the cursor position to the start point of the input field.
    move(txtbox->YPOS - 1, txtbox->XPOS);

    for(size_t i = 0; i < txtbox->WIDTH; ++i)
        addch('_');
}
