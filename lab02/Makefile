# COMP1521 19t2 ... lab 2 Makefile

CC	= gcc
CFLAGS	= -Wall -Werror -std=gnu11 -g

.PHONY:	all
all:	bits add

bits:	bits.o
add:	add.o BigNum.o

add.o:		add.c BigNum.h
BigNum.o:	BigNum.c BigNum.h

.PHONY: clean
clean:
	-rm -f bits bits.o
	-rm -f add add.o BigNum.o
	-rm -f tests/*.out

.PHONY: give
give: BigNum.c
	give cs1521 lab02 $^
