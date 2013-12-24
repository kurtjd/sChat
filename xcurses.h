/* This is just a simple way to ensure cross-compatibility. ALWAYS include
 * this library instead of the typical ncurses.h */

#ifndef XCURSES_H
#define XCURSES_H

#ifdef WIN32
#include <curses.h>
#else
#include <ncurses.h>
#endif

#endif // XCURSES_H
