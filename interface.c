#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "xcurses.h"
#include "helper.h"
#include "interface.h"
#include "message.h"
#include "txtfield.h"

#define INPUT_HEIGHTZ 2  // Temporary for history functions.

void init_curses(void)
{
    initscr();
    cbreak();
    noecho();  // Gotta turn @echo off!
    keypad(stdscr, 1);
    nodelay(stdscr, 1);  // Makes getch() non-blocking.
}

void show_message_history(const MessageHistory *messages, int *hist_start)
{
    if(messages == NULL)
        clean_exit(EXIT_FAILURE, NULL);

    /* This is done so the message history isn't redrawn every cycle.
     * If prev_last_msg doesn't hold the same address as the current last_msg,
     * we can conclude a new message was added. */
    /*static Message *prev_last_msg = NULL;
    if(prev_last_msg == messages->last_msg)
        return; */

    static int screenfull = 0;  // Is the screen full of messages?

    int maxlines = LINES - (INPUT_HEIGHTZ + 1);
    int starty = screenfull ? maxlines : 0;

    int line_on = 0;  // Used to determine when to start showing message history.

    move(0, 0);  // Position cursor
    
    /* Loop through each message by following the chain until we hit a null pointer.
     * Whether or not the screen is full determines if we go backwards or forwards. */
    // Note: if the max history is really short, when a long message is deleted the screen needs to be cleared.
    Message *msg = screenfull ? messages->last_msg : messages->first_msg;
    while(msg) {
        char *final_msg = format_message(messages, msg->sender, msg->timestamp, msg->txt);
        int msglines = (strlen(final_msg) / COLS) + 1;

        if(screenfull) {
            print_lines(final_msg, msglines, &starty, &line_on, *hist_start);
        } else {
           /* If the user tried to change the history start position while the screen
             * wasn't full, this puts it back to 0. It would make more sense for this check
             * to be in change_hist_start(), but since screenfull is not global, there
             * would be no easy way to check if the screen is full. */
            *hist_start = 0;
            
            print_message(final_msg, msglines, maxlines, &screenfull, &starty);

            // If print_message() determined the screen was full, exit the function.
            if(screenfull)
                return;
        }

        free(final_msg);
        msg = screenfull ? msg->prev_msg : msg->next_msg;
    }

    /* If the window was made larger, and the entire history
     * now fits on the screen, we need to change screenfull back to 0. */
    if(screenfull && starty > 0) {
        clear();  // If we don't, the old messages will still be at the bottom.
        screenfull = 0;
    }

    // This is part of the check to see if a new message was sent.
    //prev_last_msg = messages->last_msg;
}

void handle_input(TxtField *txtbox, MessageHistory *messages, int *hist_start, int *prev_msg_on)
{
    if(messages == NULL)
        clean_exit(EXIT_FAILURE, NULL);

    int keyp = getch();

    if(keyp == ERR)
        return;  // No key pressed.

    // Enter key returns '\n', and don't send blank messages.
    if(keyp == '\n' && txtbox->length > 0) {
        // Just a quick and temporary solution to allow for clean exit.
        if(strcmp(txtbox->contents, "/q") == 0)
            clean_exit(EXIT_SUCCESS, messages);

        add_message(messages, FROM_SELF, time(0), txtbox->contents);
        *prev_msg_on = 0;
        tf_clear(txtbox);
    } else if(keyp == KEY_BACKSPACE || keyp == 127 || keyp == 8)
        // There are multiple keys representing backspace.
        tf_backspace(txtbox);
    else if(isprint(keyp))
        tf_insert(txtbox, keyp);
        
    else if(keyp == KEY_C_UP)
        change_hist_start(1, hist_start, messages);
    else if(keyp == KEY_C_DOWN)
        change_hist_start(-1, hist_start, messages);

    else if(keyp == KEY_LEFT)
        tf_move_cursor(txtbox, -1);
    else if(keyp == KEY_RIGHT)
        tf_move_cursor(txtbox, 1);

    else if (keyp == KEY_UP)
        cycle_sent_msg(1, messages, txtbox, prev_msg_on);
    else if (keyp == KEY_DOWN)
        cycle_sent_msg(-1, messages, txtbox, prev_msg_on);

    else if(keyp == KEY_RESIZE) {
        window_resize();
        tf_scale(txtbox, 0, LINES - 1, COLS);
        tf_reset_echo(txtbox);
    }
}

void print_lines(const char *msg, const int msglines, int *starty, int *line_on, const int hist_start)
{
    /* Display each line from the bottom-up.
     * starty is checked because attempting to move the cursor
     * beyond the screen and then drawing causes problems. */
    for(int line = msglines; line > 0 && *starty >= 0; --line) {
        if(*line_on >= hist_start) {
            // Position cursor and clear all previous text on the line.
            move((*starty)--, 0);
            clrtoeol();

            /* Display at most COLS characters of the line, and determine
            * which character in the line starts a newline. */
            printw("%.*s", COLS, msg + ((line - 1) * COLS));
        }

        ++*line_on;
    }
}

void print_message(char *msg, const int msglines, const int maxlines, int *screenfull, int *starty)
{
    *starty += msglines;

    if(*starty > maxlines) {
        *screenfull = 1;
        free(msg);
        return;
    }

    // Lines break automatically when printing downward, so no need for anything fancy.
    printw("%s\n", msg);
}

void window_resize(void)
{
    endwin();
    refresh();
    clear();
}

void change_hist_start(const int dir, int *hist_start, const MessageHistory *messages)
{
    // Max amount of lines that can fit on screen.
    int maxlines = LINES - (INPUT_HEIGHTZ + 1);

    if(dir < 0 && *hist_start > 0)
        --*hist_start;
    // This long condition is to make sure hist_start doesn't go out of bounds.
    else if(dir > 0 && *hist_start < ((get_hist_lines_total(messages) - maxlines) - 1))
        ++*hist_start;
}

int get_hist_lines_total(const MessageHistory *messages)
{
    int total_lines = 0;

    for(Message *msg = messages->first_msg; msg != NULL; msg = msg->next_msg) {
        char *final_msg = format_message(messages, msg->sender, msg->timestamp, msg->txt);
        total_lines += ((strlen(final_msg) / COLS) + 1);
        free(final_msg);
    }

    return total_lines;
}

void cycle_sent_msg(const int dir, const MessageHistory *messages, TxtField *txtbox, int *prev_msg_on)
{
    // Cycle between previously sent messages.
    if(dir < 0) {
        if(*prev_msg_on > 0)
            --*prev_msg_on;
        else
            *prev_msg_on = messages->msg_count;
    } else if(dir > 0) {
        if(*prev_msg_on < messages->msg_count)
            ++*prev_msg_on;
        else
            *prev_msg_on = 0;
    }

    // Clear the message buffer and text in the input field.
    tf_clear(txtbox);

    /* Loop through the previous messages until we find the one that matches what the user wants,
     * and then place it into the message buffer. */
    int i = 0;
    for(Message *msg = messages->last_msg; i != *prev_msg_on; msg = msg->prev_msg) {
        if(msg->sender == FROM_SELF)
            ++i;

        if(i == *prev_msg_on)
            tf_set(txtbox, msg->txt);
    }
    
    // Need to reset echo_start in case the message is longer than the screen.
    tf_reset_echo(txtbox);
}
