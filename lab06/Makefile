# COMP1521 19t2 ... lab 6 Makefile

CC	= gcc
CFLAGS	= -Wall -Werror -std=gnu11 -g

.PHONY:	all
all:	stat myls

stat:	stat.o
myls:	myls.o

ifeq ($(shell uname),Linux)
myls:	LDLIBS=-lbsd
endif

.PHONY: clean
clean:
	-rm -f stat stat.o
	-rm -f myls myls.o
	-rm -f tests/*.out

.PHONY: give
give: myls.c
	give cs1521 lab06 $^
