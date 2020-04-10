// Identity matrix check
//
// requires N and m[N][N] matrix

#include <stdio.h>

#include "matrix.h" // defines N and m[N][N]

void showMatrix (int m[N][N], int n);
int isIdent (int m[N][N], int n);

int main (void)
{
	printf ("The matrix\n");
	showMatrix (m, N);
	if (isIdent (m, N))
		printf ("is an identity matrix\n");
	else
		printf ("is not an identity matrix\n");
	return 0;
}

// display a n x n matrix row-by-row
void showMatrix (int m[N][N], int n)
{
	for (int row = 0; row < n; row++) {
		for (int col = 0; col < n; col++) {
			putchar (' ');
			printf ("%d", m[row][col]);
		}
		printf ("\n");
	}
}

int isIdent (int m[N][N], int n)
{
	for (int row = 0; row < n; row++)
		for (int col = 0; col < n; col++)
			if (row == col && m[row][col] != 1)
				return 0;
			else if (row != col && m[row][col] != 0)
				return 0;
	return 1;
}
