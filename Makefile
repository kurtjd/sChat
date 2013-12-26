CC=gcc
FLAGS=-Wall -Wextra -pedantic -Werror -std=gnu99
LIB=-lncurses

all:
	$(CC) $(FLAGS) schat.c interface.c network.c message.c helper.c interface.h network.h message.h helper.h xcurses.h $(LIB) -o schat.o
