#include <stdlib.h>
#include <string.h>
#include "txtfield.h"
#include "xcurses.h"
#include "helper.h"

#define ECHO_OFFSET (txtbox->SCROLL_GAP * txtbox->echo_start)
#define TF_CURSOR_POS (get_cursor(X) - txtbox->XPOS)

void tf_init(TxtField *txtbox, const unsigned xpos, const unsigned ypos, const size_t width, const size_t maxlen)
{
    tf_scale(txtbox, xpos, ypos, width);
    txtbox->HEIGHT = 1;

    txtbox->MAXLEN = maxlen;
    txtbox->SCROLL_GAP = 20;

    txtbox->contents = malloc(maxlen + 1);  // MAKE SURE TO FREE ON EXIT!
    txtbox->length = 0;

    txtbox->echo_start = 0;
    txtbox->cursor_offset = 0;
}

void tf_insert(TxtField *txtbox, const char c)
{
    if(txtbox->length >= (txtbox->MAXLEN - 1))
        return;

    // The index in the message buffer c is to be inserted.
    unsigned insert_at = TF_CURSOR_POS + ECHO_OFFSET;

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

void tf_backspace(TxtField *txtbox)
{
    // Don't want this function running if there is no input.
    if(txtbox->length < 1)
        return;

    // The index in the message buffer to perform deletion.
    unsigned delete_at = (TF_CURSOR_POS - 1) + ECHO_OFFSET;

    for(size_t i = delete_at; i < txtbox->length; ++i)
        txtbox->contents[i] = txtbox->contents[i + 1];

    moveby(0, -1);
    delch();
    --txtbox->length;

    if(get_cursor(X) <= txtbox->XPOS)
        tf_reset_echo(txtbox);
        /* Remember: This can cause prompt to be removed by tf_backspace if cursor offset is not 0.
         * Will fix after fixing the issue with inserting text in the middle of the message. */
}

void tf_clear(TxtField *txtbox)
{
    move(txtbox->YPOS, txtbox->XPOS);

    clrtoeol();

    txtbox->echo_start = 0;
    txtbox->cursor_offset = 0;

    txtbox->contents[0] = '\0';
    txtbox->length = 0;
}

void tf_echo(TxtField *txtbox)
{
    move(txtbox->YPOS, txtbox->XPOS);
    printw("%s", txtbox->contents + ECHO_OFFSET);

    // Moves the cursor back to it's offset position if the user has moved it.
    tf_move_cursor(txtbox, 0);
}

void tf_reset_echo(TxtField *txtbox)
{
    move(txtbox->YPOS, txtbox->XPOS);
    clrtoeol();
    txtbox->echo_start = 0;

    /* Basically calculates how many times the user would've typed past the width
     * of the text field, and then sets echo_start to that value. */
    if(txtbox->length >= txtbox->WIDTH)
        txtbox->echo_start = 1 + ((txtbox->length - (txtbox->WIDTH)) / txtbox->SCROLL_GAP);
}

void tf_move_cursor(TxtField *txtbox, const int dir)
{
    if(dir < 0 && get_cursor(X) > txtbox->XPOS)
        --txtbox->cursor_offset;
    else if(dir > 0 && txtbox->cursor_offset < 0)
        ++txtbox->cursor_offset;

    move(txtbox->YPOS, get_cursor(X) + txtbox->cursor_offset);
}

void tf_scale(TxtField *txtbox, const unsigned newx, const unsigned newy, const unsigned newwidth)
{
    txtbox->XPOS = newx;
    txtbox->YPOS = newy;
    txtbox->WIDTH = newwidth;
}

void tf_draw(const TxtField *txtbox)
{
    move(txtbox->YPOS - 1, txtbox->XPOS);

    for(size_t i = 0; i < txtbox->WIDTH; ++i)
        addch('_');
}
