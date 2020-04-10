# COMP1521 19t2 ... lab 3 Makefile

CC	= gcc
CFLAGS	= -Wall -Werror -std=gnu11 -g

.PHONY: all
all:	watb maf

watb:	watb.o
watb.o:	watb.c

maf:	maf.o
maf.o:	maf.c

.PHONY: clean
clean:
	-rm -f watb watb.o
	-rm -f maf maf.o
	-rm -f tests/*.out

.PHONY: give
give: watb.c maf.c
	give cs1521 lab03 $^
