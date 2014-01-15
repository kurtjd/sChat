#ifndef TXTFIELD_H
#define TXTFIELD_H

#include <string.h>

typedef struct
{
    unsigned XPOS;
    unsigned YPOS;
    size_t WIDTH;
    size_t HEIGHT;

    size_t MAXLEN;
    size_t SCROLL_GAP;  // How much space the textfield should scroll by when input reaches width.

    char *contents;
    size_t length;

    unsigned echo_start;  // The position in the input to begin echoing.
    int cursor_offset;  // Cursor position relative to end of input.
} TxtField;

// Initializes a new text field.
void tf_init(TxtField *tf, const unsigned xpos, const unsigned ypos, const size_t width, const size_t maxlen);

// Performs cleanup.
void tf_destroy(TxtField *tf);

// Inserts a character into the textfield at the current cursor position.
void tf_insert(TxtField *tf, const char c);

// Sets the contents of the textfield to a specified string.
void tf_set(TxtField *tf, const char *str);

// Deletes the char before the cursor from textfield and from the screen.
void tf_backspace(TxtField *tf);

// Clears the textfield and the echo'd input from the screen.
void tf_clear(TxtField *tf);

// Echo back what the user is currently typing.
void tf_draw(TxtField *tf);

// Recalculates the echo_start variable.
void tf_reset_echo(TxtField *tf);

// Moves the cursor for input. Called when the left or right key is pressed.
void tf_move_cursor(TxtField *tf, const int dir);

// Resizes and repositions the text field.
void tf_scale(TxtField *tf, const unsigned newx, const unsigned newy, const unsigned newwidth);

// Draws a horizontal line across the screen, separating the message pane from the input 'field'.
void tf_draw_border(const TxtField *tf);

#endif
