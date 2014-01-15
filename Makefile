CC=gcc
FLAGS=-Wall -Wextra -pedantic -Werror -Wfatal-errors -std=gnu99
LIB=-lncurses

all:
	$(CC) $(FLAGS) schat.c interface.c txtfield.c scrollpane.c network.c message.c helper.c linkedlist.c interface.h txtfield.h scrollpane.h network.h message.h helper.h linkedlist.h xcurses.h $(LIB) -o schat
