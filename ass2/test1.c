// COMP1521 19t2 ... Assignment 2: heap management system
// test1.c: init and dump heap.

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

#include "myHeap.h"

int main (int argc, char *argv[])
{
	if (argc < 2)
		errx (EX_USAGE, "usage: %s <size>", argv[0]);

	int heapSize = atoi (argv[1]);
	if (initHeap (heapSize) < 0)
		errx (EX_SOFTWARE, "can't init heap of size %d", heapSize);

	dumpHeap ();

	return EXIT_SUCCESS;
}
