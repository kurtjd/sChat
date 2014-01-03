/* This is just a simple way to ensure cross-compatibility. ALWAYS include
 * this header instead of the typical ncurses.h */

#ifndef XCURSES_H
#define XCURSES_H

#ifdef WIN32
#include <curses.h>
#else
#include <ncurses.h>
#endif

// Might have to change these depending on OS. Not sure if these key codes are portable.
#define KEY_C_UP 565
#define KEY_C_DOWN 524

#endif // XCURSES_H
