# COMP1521 19t2 ... lab 8 Makefile

CC	= gcc
CFLAGS	= -Wall -Werror -std=gnu11 -g

.PHONY:	all
all:	mysh
mysh:	mysh.o

.PHONY: clean
clean:
	-rm -f mysh mysh.o

.PHONY: give
give: mysh.c
	give cs1521 lab08 $^
