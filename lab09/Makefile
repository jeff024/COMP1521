# COMP1521 19t2 ... lab 9 Makefile

CC	= gcc
CFLAGS	= -Wall -Werror -std=gnu11 -g

.PHONY:	all
all:	myhttpd
myhttpd:	myhttpd.o

.PHONY: clean
clean:
	-rm -f myhttpd myhttpd.o

.PHONY: give
give: myhttpd.c
	give cs1521 lab09 $^
