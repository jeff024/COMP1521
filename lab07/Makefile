# COMP1521 19t2 ... lab 7 Makefile

CC	= gcc
CFLAGS	= -Wall -Werror -std=gnu11 -g

.PHONY:	all
all:	vmsim

vmsim:	vmsim.o

.PHONY: clean
clean:
	-rm -f vmsim vmsim.o
	-rm -f tests/*.out

.PHONY: give
give: vmsim.c
	give cs1521 lab07 $^
