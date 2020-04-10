// BigNum.c ... LARGE positive integer values

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BigNum.h"

// Initialise a BigNum to N bytes, all zero
void initBigNum(BigNum *bn, int Nbytes)
{
	bn->nbytes = Nbytes;
	bn->bytes = calloc(sizeof(Byte), bn->nbytes);
	for (int i = 0; i < bn->nbytes; i++)
	{
		bn->bytes[i] = '0';
	}
	assert(bn->bytes != NULL);
}

static int ctoi(char c)
{
	return c - '0';
}

static char itoc(int num)
{
	return '0' + num;
}

// Add two BigNums and store result in a third BigNum
void addBigNums(BigNum bnA, BigNum bnB, BigNum *res)
{
	if (ctoi(bnA.bytes[0]) + ctoi(bnB.bytes[0]) > 8 ||
		res->nbytes < bnA.nbytes ||
		res->nbytes < bnB.nbytes)
	{
		res->nbytes = bnA.nbytes + bnB.nbytes;
		res->bytes = realloc(res->bytes, sizeof(Byte) * res->nbytes);
		for (int i = 0; i < res->nbytes; i++)
		{
			res->bytes[i] = '0';
		}
		assert(res->bytes != NULL);
	}
	int na1, nb1;
	int tmp, add;
	char cha;
	add = 0;

	for (int i = res->nbytes - 1,
			 j = bnA.nbytes - 1,
			 k = bnB.nbytes - 1;
		 i >= 0;
		 i--, j--, k--)
	{
		if (j < 0 && k >= 0)
		{
			na1 = 0;
			nb1 = ctoi(bnB.bytes[k]);
		}
		else if (k < 0 && j >= 0)
		{
			nb1 = 0;
			na1 = ctoi(bnA.bytes[j]);
		}
		else if (j < 0 && k < 0)
			break;
		else
		{
			na1 = ctoi(bnA.bytes[j]);
			nb1 = ctoi(bnB.bytes[k]);
		}
		tmp = na1 + nb1 + add;
		if (tmp > 9)
		{
			add = 1;
			cha = itoc(tmp - 10);
			res->bytes[i] = cha;
		}
		else
		{
			cha = itoc(tmp);
			res->bytes[i] = cha;
			add = 0;
		}
	}
}

// Set the value of a BigNum from a string of digits
// Returns 1 if it *was* a string of digits, 0 otherwise
int scanBigNum(char *s, BigNum *bn)
{
	int head = 0;
	int tail = 0;
	int isvalid = 0;
	int len = strlen(s);
	char c;

	if (len > bn->nbytes)
	{
		bn->nbytes = len + 5;
		bn->bytes = realloc(bn->bytes, sizeof(Byte) * bn->nbytes);
		for (int i = 0; i < bn->nbytes; i++)
		{
			bn->bytes[i] = '0';
		}
		assert(bn->bytes != NULL);
	}

	for (int i = 0; i < len; i++)
	{
		c = s[i];
		if (isspace(c) == 0 && isdigit(c) != 0)
		{
			head = i;
			isvalid++;
			break;
		}
	}
	if (isvalid == 0)
		return 0;

	for (int i = head; i < len; i++)
	{
		if (isdigit(s[i]) == 0)
			tail = i;
	}
	if (tail == 0)
		tail = len;

	int length = tail - head;
	for (int i = bn->nbytes, q = 0; i > bn->nbytes - length - 1; i--, q++)
	{
		bn->bytes[i] = s[tail - q];
	}
	return 1;
}

// Display a BigNum in decimal format
void showBigNum(BigNum bn)
{
	int num_nonzero = 0;
	int head;
	for (int i = 0; i < bn.nbytes; i++)
	{
		if (bn.bytes[i] != '0')
			num_nonzero++;
	}

	if (num_nonzero == 0)
		printf("0");
	else
	{
		for (int i = 0; i < bn.nbytes; i++)
		{
			if (bn.bytes[i] == '0')
				continue;
			else
			{
				head = i;
				break;
			}
		}
	}
	for (int i = head; i < bn.nbytes; i++)
	{
		printf("%c", bn.bytes[i]);
	}
}
