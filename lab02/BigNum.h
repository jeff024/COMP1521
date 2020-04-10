// BigNum.h ... LARGE positive integer values

#ifndef CS1521_LAB02_BIGNUM_H_
#define CS1521_LAB02_BIGNUM_H_

typedef unsigned char Byte;

typedef struct _big_num {
	int nbytes;  // size of array
	Byte *bytes; // array of Bytes
} BigNum;

// Initialise a BigNum to N bytes, all zero
void initBigNum (BigNum *bn, int Nbytes);

// Add two BigNums and store result in a third BigNum
void addBigNums (BigNum bnA, BigNum bnB, BigNum *res);

// Set the value of a BigNum from a string of digits.
// Returns 1 if it *was* a string of digits, 0 otherwise
int scanBigNum (char *s, BigNum *bn);

// Display a BigNum in decimal format
void showBigNum (BigNum bn);

#endif // !defined CS1521_LAB02_BIGNUM_H_
