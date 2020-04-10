// COMP1521 19t2 ... lab 03: where are the bits?
// watb.c: determine bit-field order

#include <stdio.h>
#include <stdlib.h>

struct _bit_fields {
	unsigned int a : 4;
	unsigned int b : 8;
	unsigned int c : 20;
};

int main (void)
{
	struct _bit_fields x = {0, 1, 0};
    unsigned int *ptrX = (unsigned int *) &x;
    unsigned int mask = 1<<31;
    
    unsigned int size = 0;

    for (int i = 0; i < 32; i ++) {
        if (mask & *ptrX) {
            size = size | mask;
        }
        mask = mask >> 1;
    }
    
	printf ("%u\n", size);

	return EXIT_SUCCESS;
}

