CC=gcc
FLAGS=-Wall -Wextra -std=gnu99
LIB=-lncurses

all:
	$(CC) $(FLAGS) schat.c helper.c helper.h network.c network.h $(LIB) -o schat.o
