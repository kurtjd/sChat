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
void tf_init(TxtField *txtbox, const unsigned xpos, const unsigned ypos, const size_t width, const size_t maxlen);

// Inserts a character into the textfield at the current cursor position.
void tf_insert(TxtField *txtbox, const char c);

// Sets the contents of the textfield to a specified string.
void tf_set(TxtField *txtbox, const char *str);

// Deletes the char before the cursor from textfield and from the screen.
void tf_backspace(TxtField *txtbox);

// Clears the textfield and the echo'd input from the screen.
void tf_clear(TxtField *txtbox);

// Echo back what the user is currently typing.
void tf_echo(TxtField *txtbox);

// Recalculates the echo_start variable.
void tf_reset_echo(TxtField *txtbox);

// Moves the cursor for input. Called when the left or right key is pressed.
void tf_move_cursor(TxtField *txtbox, const int dir);

// Resizes and repositions the text field.
void tf_scale(TxtField *txtbox, const unsigned newx, const unsigned newy, const unsigned newwidth);

// Draws a horizontal line across the screen, separating the message pane from the input 'field'.
void tf_draw(const TxtField *txtbox);

#endif
