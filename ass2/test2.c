// COMP1521 19t2 ... Assignment 2: heap management system
// test2.c: insert random values into a sorted linked list

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

#include "myHeap.h"

typedef struct _node {
	int data;
	struct _node *next;
} Node;

typedef Node *List;

List insert (List, int);
void showList (List);
void freeList (List);

// This `#define' allows you to use your own `myMalloc' and `myFree'
// implementations -- comment it out or remove it to use plain old
// `malloc(3)' and `free(3)' and to see the expected behaviour.
#define USE_MYHEAP

#ifdef USE_MYHEAP
# define MEM_ALLOC myMalloc
# define MEM_FREE  myFree
#else
# define MEM_ALLOC malloc
# define MEM_FREE  free
#endif

int main (void)
{
#ifdef USE_MYHEAP
	initHeap (10000);
#endif

	List list = NULL;
	for (int i = 0; i < 100; i++) {
		list = insert (list, rand () % 100);
		printf ("L = ");
		showList (list);
#ifdef USE_MYHEAP
		dumpHeap();
#endif
	}

	freeList (list);
	printf ("After freeList ...\n");

#ifdef USE_MYHEAP
	dumpHeap ();
	freeHeap ();
#endif

	return EXIT_SUCCESS;
}

List insert (List L, int n)
{
	Node *new = MEM_ALLOC (sizeof (Node));
	if (new == NULL) errx (1, "couldn't allocate Node");
	new->data = n;
	Node *prev = NULL;
	Node *curr = L;
	while (curr != NULL) {
		if (n < curr->data)
			break;
		prev = curr;
		curr = curr->next;
	}
	if (prev == NULL) { // new front
		new->next = curr;
		return new;
	} else { // insert in middle
		new->next = prev->next;
		prev->next = new;
		return L;
	}
}

void showList (List L)
{
	while (L != NULL) {
		printf ("%d", L->data);
		if (L->next != NULL)
			printf ("->");
		L = L->next;
	}
	printf ("\n");
}

void freeList (List L)
{
	if (L == NULL) return;
	freeList (L->next);
	MEM_FREE (L);
}
