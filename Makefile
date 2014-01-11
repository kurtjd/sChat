CC=gcc
FLAGS=-Wall -Wextra -pedantic -Werror -Wfatal-errors -std=gnu99
LIB=-lncurses

all:
	$(CC) $(FLAGS) schat.c interface.c txtfield.c network.c message.c helper.c interface.h txtfield.h network.h message.h helper.h xcurses.h $(LIB) -o schat
