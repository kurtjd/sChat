#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "xcurses.h"
#include "interface.h"
#include "txtfield.h"
#include "scrollpane.h"
#include "message.h"
#include "linkedlist.h"
#include "helper.h"

void init_curses(void)
{
    initscr();
    cbreak();
    noecho();  // Gotta turn @echo off!
    keypad(stdscr, 1);
    nodelay(stdscr, 1);  // Makes getch() non-blocking.
}

void handle_input(LinkedList *messages, ScrollPane *sp, TxtField *tf, unsigned *prev_msg_on)
{
    if (messages == NULL)
        return;

    int keyp = getch();

    switch (keyp) {
    case ERR:  // No key pressed.
        return;

    case '\n':  // This means the enter key was pressed.
        // Don't send blank messages.
        if (tf->length < 1)
            break;

        // Just a quick and temporary solution to allow for clean exit.
        if (strcmp(tf->value, "/q") == 0)
            clean_exit(EXIT_SUCCESS, messages, sp, tf);

        // Add the message to history then print it.
        Message *msg = msg_new(messages, FROM_SELF, time(0), tf->value);
        msg_print(msg, sp);

        *prev_msg_on = 0;
        tf_clear(tf);
        break;

    // There are multiple keys representing backspace.
    case KEY_BACKSPACE:
    case 127:
    case 8:
        tf_backspace(tf);
        break;

    case KEY_C_UP:
        sp_scroll(sp, 1);
        msg_print_all(messages, sp);
        break;

    case KEY_C_DOWN:
        sp_scroll(sp, -1);
        msg_print_all(messages, sp);
        break;

    case KEY_LEFT:
        tf_move_cursor(tf, -1);
        break;

    case KEY_RIGHT:
        tf_move_cursor(tf, 1);
        break;

    case KEY_UP:
        cycle_sent_msg(1, messages, tf, prev_msg_on);
        break;

    case KEY_DOWN:
        cycle_sent_msg(-1, messages, tf, prev_msg_on);
        break;

    case KEY_RESIZE:
        window_resize();
        tf_scale(tf, 0, LINES - 1, COLS);
        break;

    /* If we get here, a key was pressed that we don't explicitly want to handle.
     * However if a printable key was pressed, we want to handle that. */
    default:
        if (isprint(keyp))
            tf_insert(tf, keyp);
        break;
    }
}

void window_resize(void)
{
    endwin();
    refresh();
    clear();
}

// This needs to go somewhere else. It is only temporary.
void cycle_sent_msg(const int dir, const LinkedList *messages, TxtField *tf, unsigned *prev_msg_on)
{
    // Cycle between previously sent messages.
    if (dir < 0) {
        if (*prev_msg_on >= 1)
            --*prev_msg_on;
        else
            *prev_msg_on = messages->size;
    } else if (dir > 0) {
        if (*prev_msg_on < messages->size)
            ++*prev_msg_on;
        else
            *prev_msg_on = 0;
    }

    // Clear the message buffer and text in the input field.
    tf_clear(tf);

    /* Loop through the previous messages until we find the one that matches what the user wants,
     * and then place it into the message buffer. */
    Node *node = messages->last;
    for (unsigned i = 0; i != *prev_msg_on; node = node->prev) {
        const Message *msg = node->value;
        if (msg->sender == FROM_SELF)
            ++i;
        if (i == *prev_msg_on)
            tf_set(tf, msg->txt);
    }
    
    // Need to reset echo_start in case the message is longer than the screen.
    tf_reset_echo(tf);
}
