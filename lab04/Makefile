# COMP1521 19t2 ... lab 4 Makefile
#
# We need to explicitly state the .o <- .c dependency,
# otherwise Make will try to use .o <- .s instead.
# (This will not work.)

CC	= gcc
CFLAGS	= -Wall -Werror -std=gnu11 -g

.PHONY: all
all:	fac1 fac2

fac1:	fac1.o
fac1.o:	fac1.c

fac2:	fac2.o
fac2.o:	fac2.c

.PHONY: clean
clean:
	-rm -f fac1 fac1.o
	-rm -f fac2 fac2.o
	-rm -f tests/*.out

.PHONY: give
give: fac1.s fac2.s
	give cs1521 lab04 $^
