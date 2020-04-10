# COMP1521 19t2 ... assignment 2 Makefile

CC	= gcc
CFLAGS	= -Wall -Werror -std=gnu11 -g

.PHONY:	all
all:	test1 test2 test3

test1:		test1.o myHeap.o
test1.o:	test1.c myHeap.h

test2:		test2.o myHeap.o
test2.o:	test2.c myHeap.h

test3:		test3.o myHeap.o
test3.o:	test3.c myHeap.h

myHeap.o:	myHeap.c myHeap.h


.PHONY: clean
clean:
	-rm -f test1 test1.o
	-rm -f test2 test2.o
	-rm -f test3 test3.o
	-rm -f myHeap.o

.PHONY: give
give: myHeap.c
	give cs1521 assign2 $^
