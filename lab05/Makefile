# COMP1521 19t2 ... lab 5 Makefile
#
# We need to explicitly state the .o <- .c dependency,
# otherwise Make will try to use .o <- .s instead.
# (This will not work.)

CC	= gcc
CFLAGS	= -Wall -Werror -std=gnu11 -g

.PHONY: all
all:	fac3 isi

fac3:	fac3.o
fac3.o:	fac3.c

isi:	isi.o
isi.o:	isi.c matrix.h

.PHONY: clean
clean:
	-rm -f fac3 fac3.o
	-rm -f isi isi.o
	-rm -f tests/*.out

.PHONY: give
give: fac3.s isi.s
	give cs1521 lab05 $^
