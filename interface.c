#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "xcurses.h"
#include "helper.h"
#include "interface.h"
#include "message.h"


void init_curses(int *screen_h, int *screen_w)
{
    if(screen_h == NULL || screen_w == NULL)
        clean_exit(EXIT_FAILURE, NULL);

    initscr();
    cbreak();
    noecho();  // Gotta turn @echo off!
    keypad(stdscr, 1);
    nodelay(stdscr, 1);  // Makes getch() non-blocking.

    /* Dereference here because getmaxyx(), like many ncurses functions,
     * is actually a macro. This essentially expands to:
     *
     * *screen_h = ROWS;
     * *screen_w = COLS;   */
    getmaxyx(stdscr, *screen_h, *screen_w);
}


void show_message_history(const MessageHistory *messages, const int screen_h, const int screen_w, int *hist_start)
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

    int maxlines = screen_h - (INPUT_HEIGHT + 1);
    int starty = screenfull ? maxlines : 0;

    int line_on = 0;  // Used to determine when to start showing message history.

    move(0, 0);  // Position cursor
    
    /* Loop through each message by following the chain until we hit a null pointer.
     * Whether or not the screen is full determines if we go backwards or forwards. */
    // Note: if the max history is really short, when a long message is deleted the screen needs to be cleared.
    Message *msg = screenfull ? messages->last_msg : messages->first_msg;
    while(msg)
    {
        char *final_msg = format_message(messages, msg->sender, msg->timestamp, msg->txt);
        int msglines = (strlen(final_msg) / screen_w) + 1;

        if(screenfull)
            print_lines(final_msg, msglines, screen_w, &starty, &line_on, *hist_start);
        else
        {
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
    if(screenfull && starty > 0)
    {
        clear();  // If we don't, the old messages will still be at the bottom.
        screenfull = 0;
    }

    //prev_last_msg = messages->last_msg;
}


void draw_input_field(const int length, const int screen_h)
{
    // Sets the cursor position to the y offset.
    move(screen_h - INPUT_HEIGHT, 0);

    for(int i = 0; i < length; ++i)
        addch('_');

    // Just add a 'prompt' character for stylistic reasons.
    printw("> ");
}


void echo_user_input(const char *msgbuf, const unsigned screen_h, const int echo_start, int *cursor_offset)
{
    move(screen_h - 1, PROMPT_LEN);
    printw("%s", msgbuf + (SCROLL_GAP * echo_start));

    // Moves the cursor back to it's offset position if the user has moved it.
    move_cursor(0, cursor_offset, strlen(msgbuf), echo_start);
}


void change_echo_start(int *echo_start, const int dir, const int screen_w)
{
    if(dir > 0)
        *echo_start += dir;
    else
    {
        // If the user backspaced a lot, need to recalculate echo_start.
        *echo_start -= ((screen_w - PROMPT_LEN) / SCROLL_GAP);
    }

    // echo_start should always be 0 at a minimum.
    if(*echo_start < 0)
        *echo_start = 0;

    // Move the cursor to the beginning of the input to clear it.
    move(getcury(stdscr), PROMPT_LEN + 1);
    clrtoeol();
}


void handle_input(char *msgbuf, MessageHistory *messages, int *screen_h, int *screen_w, int *echo_start, int *cursor_offset, int *hist_start)
{
    if(messages == NULL)
        clean_exit(EXIT_FAILURE, NULL);

    size_t msglen = strlen(msgbuf);
    int keyp = getch();

    // Get the cursor x coordinate.
    int cursx = getcurx(stdscr);

    if(keyp == ERR)
        return;  // No key pressed.

    // Enter key returns '\n', and don't send blank messages.
    if(keyp == '\n' && msglen > 0)
    {
        // Just a quick and temporary solution to allow for clean exit.
        if(strcmp(msgbuf, "/q") == 0)
            clean_exit(EXIT_SUCCESS, messages);

        add_message(messages, FROM_SELF, time(0), msgbuf);
        clear_input(msgbuf, echo_start, cursor_offset);
    }

    // There are multiple keys representing backspace.
    else if(keyp == KEY_BACKSPACE || keyp == 127 || keyp == 8)
    {
        backspace(msgbuf, *echo_start);

        if(cursx == (PROMPT_LEN + 1))
            change_echo_start(echo_start, -1, *screen_w);
    }
    else if(isprint(keyp))
    {
        if(msglen < (MAX_MSG_LEN - 1))
        {
            insert_char(msgbuf, keyp, *echo_start);

            if(cursx == (*screen_w - 1))
                change_echo_start(echo_start, 1, *screen_w);
        }
    }

    else if(keyp == KEY_UP)
        change_hist_start(1, hist_start, *screen_h, *screen_w, messages);
    
    else if(keyp == KEY_DOWN)
        change_hist_start(-1, hist_start, *screen_h, *screen_w, messages);

    else if(keyp == KEY_LEFT)
        move_cursor(-1, cursor_offset, strlen(msgbuf), *echo_start);

    else if(keyp == KEY_RIGHT)
        move_cursor(1, cursor_offset, strlen(msgbuf), *echo_start);

    else if(keyp == KEY_RESIZE)
        window_resize(screen_h, screen_w);
}


void print_lines(const char *msg, const int msglines, const int screen_w, int *starty, int *line_on, const int hist_start)
{
    /* Display each line from the bottom-up.
     * starty is checked because attempting to move the cursor
     * beyond the screen and then drawing causes problems. */
    for(int line = msglines; line > 0 && *starty >= 0; --line)
    {
        if(*line_on >= hist_start)
        {
            // Position cursor and clear all previous text on the line.
            move((*starty)--, 0);
            clrtoeol();

            /* Display at most screen_w characters of the line, and determine
            * which character in the line starts a newline. */
            printw("%.*s", screen_w, msg + ((line - 1) * screen_w));
        }

        ++*line_on;
    }
}


void print_message(char *msg, const int msglines, const int maxlines, int *screenfull, int *starty)
{
    *starty += msglines;

    if(*starty > maxlines)
    {
        *screenfull = 1;
        free(msg);
        return;
    }

    // Lines break automatically when printing downward, so no need for anything fancy.
    printw("%s\n", msg);
}


void window_resize(int *screen_h, int *screen_w)
{
    endwin();
    refresh();
    getmaxyx(stdscr, *screen_h, *screen_w);
    clear();
}


void clear_input(char *msgbuf, int *echo_start, int *cursor_offset)
{
    // Moves the cursor to the beginning of the echo'd input.
    move(getcury(stdscr), PROMPT_LEN + 1);

    // Clears all text from the cursor to the end of line.
    clrtoeol();

    *echo_start = 0;  // Reset the echo start position.
    *cursor_offset = 0;  // Reset cursor offset.

    msgbuf[0] = '\0';  // Clear the buffer.
}


void backspace(char *msgbuf, const int echo_start)
{
    // Don't want this function running if there is no input.
    if(strlen(msgbuf) < 1)
        return;

    // The index in the message buffer to perform deletion.
    unsigned insert_at = ((getcurx(stdscr) - PROMPT_LEN) - 1) + (SCROLL_GAP * echo_start);
    size_t msglen = strlen(msgbuf);

    for(size_t i = insert_at; i < msglen; ++i)
        msgbuf[i] = msgbuf[i + 1];

    // Moves the cursor to the left then deletes the character under it.
    moveby(0, -1);
    delch();
}


void move_cursor(const int dir, int *cursor_offset, const int xmax, const int echo_start)
{
    int cursy, cursx;
    getyx(stdscr, cursy, cursx);

    if(dir < 0 && cursx > PROMPT_LEN)
        --*cursor_offset;
    else if(dir > 0 && cursx < PROMPT_LEN + xmax - (SCROLL_GAP * echo_start))
        ++*cursor_offset;

    move(cursy, cursx + *cursor_offset);
}


void insert_char(char *msgbuf, const char c, const int echo_start)
{
    // The index in the message buffer c is to be inserted.
    unsigned insert_at = (getcurx(stdscr) - PROMPT_LEN) + (SCROLL_GAP * echo_start);
    size_t msglen = strlen(msgbuf);

    // Move characters to the right that are after the insertion point.
    for(size_t i = msglen; i > insert_at; --i)
        msgbuf[i] = msgbuf[i - 1];

    msgbuf[insert_at] = c;
    msgbuf[msglen + 1] = '\0';
}


void change_hist_start(const int dir, int *hist_start, const int screen_h, const int screen_w, const MessageHistory *messages)
{
    // Max amount of lines that can fit on screen.
    int maxlines = screen_h - (INPUT_HEIGHT + 1);

    if(dir < 0 && *hist_start > 0)
        --*hist_start;
    // This long condition is to make sure hist_start doesn't go out of bounds.
    else if(dir > 0 && *hist_start < ((get_hist_lines_total(messages, screen_w) - maxlines) - 1))
        ++*hist_start;
}


int get_hist_lines_total(const MessageHistory *messages, const int screen_w)
{
    int total_lines = 0;

    for(Message *msg = messages->first_msg; msg != NULL; msg = msg->next_msg)
    {
        char *final_msg = format_message(messages, msg->sender, msg->timestamp, msg->txt);
        total_lines += ((strlen(final_msg) / screen_w) + 1);
        free(final_msg);
    }

    return total_lines;
}
