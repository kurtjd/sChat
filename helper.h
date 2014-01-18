/******************************************************************************
 * helper.h: Miscellaneous functions used by other modules.                   *
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
#ifndef HELPER_H
#define HELPER_H

#include <stdlib.h>
#include "txtfield.h"
#include "scrollpane.h"
#include "linkedlist.h"

#define X 0
#define Y 1

// Performs cleanup then exits program.
void clean_exit(const int status, LinkedList *messages, ScrollPane *sp, TxtField *tf);

// Moves the cursor by specified amount.
void moveby(const int yinc, const int xinc);

// Returns either the X or Y position of the cursor.
unsigned get_cursor(const unsigned axis);

#endif
