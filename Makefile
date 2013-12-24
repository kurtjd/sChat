CC=gcc
FLAGS=-Wall -Wextra -std=gnu99
LIB=-lncurses

all:
	$(CC) $(FLAGS) schat.c helper.c helper.h $(LIB) -o schat.o
