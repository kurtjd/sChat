#include <stdlib.h>
#include <string.h>
#include "xcurses.h"
#include "txtfield.h"
#include "helper.h"

// These calculations are used multiple times, so they are better off as macros.
#define ECHO_OFFSET (tf->SCROLL_GAP * tf->echo_start)
#define TF_CURSOR_POS (get_cursor(X) - tf->XPOS)

void tf_init(TxtField *tf, const unsigned xpos, const unsigned ypos, const size_t width, const size_t maxlen)
{
    tf->HEIGHT = 1;

    tf->MAXLEN = maxlen;
    tf->SCROLL_GAP = 20;  // May make this dynamic in the future!

    tf->value = malloc(maxlen + 1);  // Handle failure in future!
    tf->length = 0;

    tf->echo_start = 0;
    tf->cursor_offset = 0;

    tf_scale(tf, xpos, ypos, width);
}

void tf_destroy(TxtField *tf)
{
    free(tf->value);
}

void tf_insert(TxtField *tf, const char c)
{
    if (tf->length >= (tf->MAXLEN - 1))
        return;

    // The index in the message buffer c is to be inserted.
    unsigned insert_at = TF_CURSOR_POS + ECHO_OFFSET;

    // Move characters to the right that are after the insertion point.
    for (size_t i = tf->length; i > insert_at; --i)
        tf->value[i] = tf->value[i - 1];

    tf->value[insert_at] = c;
    tf->value[tf->length + 1] = '\0';
    ++tf->length;

    if (get_cursor(X) == (tf->XPOS + tf->WIDTH - 1))
        tf_reset_echo(tf);
}

void tf_set(TxtField *tf, const char *str)
{
    strncpy(tf->value, str, strlen(str) + 1);
    tf->length = strlen(str);
}

void tf_backspace(TxtField *tf)
{
    // Don't want this function running if there is no input.
    if (tf->length < 1)
        return;

    // The index in the message buffer to perform deletion.
    unsigned delete_at = (TF_CURSOR_POS - 1) + ECHO_OFFSET;

    for (size_t i = delete_at; i < tf->length; ++i)
        tf->value[i] = tf->value[i + 1];

    moveby(0, -1);
    delch();
    --tf->length;

    if (get_cursor(X) <= tf->XPOS)
        tf_reset_echo(tf);
        /* Remember: This can cause prompt to be removed by tf_backspace if cursor offset is not 0.
         * Will fix after fixing the issue with inserting text in the middle of the message. */
}

void tf_clear(TxtField *tf)
{
    move(tf->YPOS, tf->XPOS);

    clrtoeol();

    tf->echo_start = 0;
    tf->cursor_offset = 0;

    tf->value[0] = '\0';
    tf->length = 0;
}

void tf_draw(TxtField *tf)
{
    tf_draw_border(tf);

    move(tf->YPOS, tf->XPOS);
    printw("%s", tf->value + ECHO_OFFSET);

    // Moves the cursor back to it's offset position if the user has moved it.
    tf_move_cursor(tf, 0);
}

void tf_reset_echo(TxtField *tf)
{
    move(tf->YPOS, tf->XPOS);
    clrtoeol();
    tf->echo_start = 0;

    /* Basically calculates how many times the user would've typed past the width
     * of the text field, and then sets echo_start to that value. */
    if (tf->length >= tf->WIDTH)
        tf->echo_start = 1 + ((tf->length - (tf->WIDTH)) / tf->SCROLL_GAP);
}

void tf_move_cursor(TxtField *tf, const int dir)
{
    if (dir < 0 && get_cursor(X) > tf->XPOS)
        --tf->cursor_offset;
    else if (dir > 0 && tf->cursor_offset < 0)
        ++tf->cursor_offset;

    move(tf->YPOS, get_cursor(X) + tf->cursor_offset);
}

void tf_scale(TxtField *tf, const unsigned newx, const unsigned newy, const unsigned newwidth)
{
    tf->XPOS = newx;
    tf->YPOS = newy;
    tf->WIDTH = newwidth;

    tf_reset_echo(tf);
}

// Will probably get rid of this in the future.
void tf_draw_border(const TxtField *tf)
{
    move(tf->YPOS - 1, tf->XPOS);

    for (size_t i = 0; i < tf->WIDTH; ++i)
        addch('_');
}
