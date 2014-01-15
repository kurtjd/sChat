#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "xcurses.h"
#include "helper.h"
#include "interface.h"
#include "scrollpane.h"
#include "message.h"
#include "txtfield.h"

void init_curses(void)
{
    initscr();
    cbreak();
    noecho();  // Gotta turn @echo off!
    keypad(stdscr, 1);
    nodelay(stdscr, 1);  // Makes getch() non-blocking.
}

void handle_input(MessageHistory *messages, ScrollPane *sp, TxtField *tf, unsigned *prev_msg_on)
{
    if (messages == NULL)
        clean_exit(EXIT_FAILURE, NULL);

    int keyp = getch();

    switch (keyp) {
    case ERR:  // No key pressed.
        return;

    case '\n':  // This means the enter key was pressed.
        // Don't send blank messages.
        if (tf->length < 1)
            break;

        // Just a quick and temporary solution to allow for clean exit.
        if (strcmp(tf->contents, "/q") == 0)
            clean_exit(EXIT_SUCCESS, messages);

        // Add the message to history then print it.
        Message *msg = hist_add_message(messages, FROM_SELF, time(0), tf->contents);
        msg_print(messages, msg, sp);

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
        hist_print_messages(messages, sp);
        break;

    case KEY_C_DOWN:
        sp_scroll(sp, -1);
        hist_print_messages(messages, sp);
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

void cycle_sent_msg(const int dir, const MessageHistory *messages, TxtField *tf, unsigned *prev_msg_on)
{
    // Cycle between previously sent messages.
    if (dir < 0) {
        if (*prev_msg_on >= 1)
            --*prev_msg_on;
        else
            *prev_msg_on = messages->msg_count;
    } else if (dir > 0) {
        if (*prev_msg_on < messages->msg_count)
            ++*prev_msg_on;
        else
            *prev_msg_on = 0;
    }

    // Clear the message buffer and text in the input field.
    tf_clear(tf);

    /* Loop through the previous messages until we find the one that matches what the user wants,
     * and then place it into the message buffer. */
    Message *msg = messages->last_msg;
    for (unsigned i = 0; i != *prev_msg_on; msg = msg->prev_msg) {
        if (msg->sender == FROM_SELF)
            ++i;

        if (i == *prev_msg_on)
            tf_set(tf, msg->txt);
    }
    
    // Need to reset echo_start in case the message is longer than the screen.
    tf_reset_echo(tf);
}
