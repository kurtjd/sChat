#include <stdlib.h>
#include <string.h>
#include "xcurses.h"
#include "helper.h"
#include "message.h"
#include "interface.h"

void append(char *dest, const char c)
{
    size_t destlen = strlen(dest);
    dest[destlen] = c;
    dest[destlen + 1] = '\0';
}


void moveby(const int yinc, const int xinc)
{
    int cury, curx;
    getyx(stdscr, cury, curx);  // Gets current cursor position.
    move(cury + yinc, curx + xinc);
}


void clear_input(char *msgbuf, const unsigned screen_w)
{
    /* Moves the cursor to the beginning of the echo'd input.
     * If the input is shorter than the screen width, move the cursor
     * back the length of the screen. If not, just move back the length
     * of the screen. */
    if(strlen(msgbuf) < screen_w)
        moveby(0, (strlen(msgbuf) * -1));
    else
        moveby(0, (screen_w * -1) + PROMPT_LEN + 1);

    // Clears all text from the cursor to the end of line.
    clrtoeol();

    msgbuf[0] = '\0';  // Clear the buffer.
}


void backspace(char *msgbuf)
{
    // Don't want this function running if there is no input.
    if(strlen(msgbuf) < 1)
        return;

    msgbuf[strlen(msgbuf) - 1] = '\0';

    // Moves the cursor to the left then deletes the character under it.
    moveby(0, -1);
    delch();
}


void* safe_malloc(const size_t size, const MessageHistory *messages)
{
    void *newmem = malloc(size);
    if(!newmem)
        clean_exit(EXIT_FAILURE, messages);

    return newmem;
}


void clean_exit(const int status, const MessageHistory *messages)
{
    if(messages)
        /* Cast away const from messages. This is done so the other functions which
         * call clean_exit() can use a const version of messages. */
        clear_history((MessageHistory *)messages);

    endwin();
    exit(status);
}
