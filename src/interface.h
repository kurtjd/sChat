/******************************************************************************
 * interface.h: General functions for interacting with the chat interface.    *
 * Copyright (C) 2014  Kurtis Dinelle & McConnell Wade                        *
 *                                                                            *
 * This file is a part of sChat.                                              *
 *                                                                            *
 * sChat is free software: you can redistribute it and/or modify              *
 * it under the terms of the GNU General Public License as published by       *
 * the Free Software Foundation, either version 3 of the License, or          *
 * (at your option) any later version.                                        *
 *                                                                            *
 * sChat is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with sChat.  If not, see [http://www.gnu.org/licenses/].             *
 ******************************************************************************/
#ifndef INTERFACE_H
#define INTERFACE_H

#include "txtfield.h"
#include "scrollpane.h"
#include "linkedlist.h"

// Sets up the curses environment with preferred settings.
void init_curses(void);

// Handles all user input. Returns 0 on memory allocation failure, and -1 on no key pressed.
int handle_input(LinkedList *messages, ScrollPane *sp, TxtField *tf);

// Reinitializes ncurses to work with the resized window.
void window_resize(void);

#endif
