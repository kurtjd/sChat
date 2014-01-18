CC=gcc
CFLAGS=-Wall -Wextra -pedantic -Werror -Wfatal-errors -std=gnu99
SRC=schat.c interface.c txtfield.c scrollpane.c network.c message.c helper.c linkedlist.c
INC=xcurses.h interface.h txtfield.h scrollpane.h network.h message.h helper.h linkedlist.h
LIB=-lncurses

all:
	$(CC) $(CFLAGS) $(SRC) $(INC) $(LIB) -o ../schat
