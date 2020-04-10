// COMP1521 19t2 ... Assignment 2: heap management system

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "myHeap.h"

/** minimum total space for heap */
#define MIN_HEAP 4096
/** minimum amount of space for a free Chunk (excludes Header) */
#define MIN_CHUNK 32

#define ALLOC 0x55555555
#define FREE 0xAAAAAAAA

/// Types:

typedef unsigned int uint;
typedef unsigned char byte;

typedef uintptr_t addr; // an address as a numeric type

/** The header for a chunk. */
typedef struct header
{
	uint status; /**< the chunk's status -- ALLOC or FREE */
	uint size;   /**< number of bytes, including header */
	byte data[]; /**< the chunk's data -- not interesting to us */
} header;

/** The heap's state */
struct heap
{
	void *heapMem;   /**< space allocated for Heap */
	uint heapSize;   /**< number of bytes in heapMem */
	void **freeList; /**< array of pointers to free chunks */
	uint freeElems;  /**< number of elements in freeList[] */
	uint nFree;		 /**< number of free chunks */
};

/// Variables:

/** The heap proper. */
static struct heap Heap;

/// Functions:

static addr heapMaxAddr(void);
// find the smallest free chunk
static void *find_sfc(int size);
// insert a pointer into freelist by order
static void list_insert(header *Header);
// remove a pointer out of freelist by order
static void list_remove(header *Header);
// merge any two adjacent free chunks
static void merge(void);

// find the smallest free chunk
static void *find_sfc(int size)
{
	// find the first suitable free chunk
	// and set it to be smalllest_size for searching
	header *chunk = NULL;
	header *returnAddr = NULL;
	uint smallest_size = 0;
	int tmp;
	for (int i = 0; i < Heap.nFree; i++)
	{
		chunk = (header *)Heap.freeList[i];
		chunk--;
		tmp = chunk->size;
		if (tmp >= size + sizeof(header))
		{
			smallest_size = tmp;
			returnAddr = chunk;
			break;
		}
	}
	// if smallest_size == 0, then there is no suitable
	// chunk can be used. return NULL
	if (smallest_size == 0)
	{
		return NULL;
	}
	// search through the freelist to find the smalleast free chunk
	for (int i = 0; i < Heap.nFree; i++)
	{
		chunk = (header *)Heap.freeList[i];
		chunk--;
		tmp = chunk->size;
		if (tmp >= size + sizeof(header) && tmp < smallest_size)
		{
			smallest_size = tmp;
			returnAddr = chunk;
		}
	}
	// returned the header address of the found free chunk
	return (void *)returnAddr;
}

// insert a pointer into freelist by order
static void list_insert(header *Header)
{
	if (Header == NULL)
	{
		fprintf(stderr, "This header cannot be inserted\n");
		exit(EXIT_FAILURE);
	}
	int i;
	for (i = Heap.nFree - 1; i >= 0; i--)
	{
		if ((addr)Heap.freeList[i] > (addr)Header)
		{
			Heap.freeList[i + 1] = Heap.freeList[i];
		}
		else
		{
			break;
		}
	}
	Heap.freeList[i + 1] = Header;
	Heap.nFree++;
}

// remove a pointer out of freelist by order
static void list_remove(header *Header)
{
	int i;
	int pos = -1;
	// pos is the position of pointer to be deleted
	for (i = 0; i < Heap.nFree; i++)
	{
		header *chunk = Heap.freeList[i];
		if ((addr)chunk == (addr)Header)
		{
			pos = i;
			break;
		}
	}
	if (pos == -1)
	{
		fprintf(stderr, "cannot find such free header in freeList\n");
		exit(EXIT_FAILURE);
	}
	// loop from the pos to end for left shift
	for (i = pos; i < Heap.nFree - 1; i++)
	{
		Heap.freeList[i] = Heap.freeList[i + 1];
	}
	Heap.freeList[i] = NULL;
	Heap.nFree--;
}

// merge any two adjacent free chunks
static void merge(void)
{
	header *next;
	header *new;
	for (int i = 0; i < Heap.nFree - 1; i++)
	{
		next = (header *)Heap.freeList[i + 1];
		next--;
		new = (header *)Heap.freeList[i];
		new --;
		// if (addr) new + new->size == (addr)next
		// then these two free chunks are adjacent
		while ((addr) new + new->size == (addr)next)
		{
			if (Heap.nFree == 1)
				break;
			new->size += next->size;
			next++;
			list_remove(next);
			next = (header *)Heap.freeList[i + 1];
			next--;
		}
	}
}

/** Initialise the Heap. */
int initHeap(int size)
{
	// round up the size
	if (size < MIN_HEAP)
		size = MIN_HEAP;
	if (size % 4 != 0)
	{
		int tmp = (4 - size % 4) + size;
		size = tmp;
	}

	// initialise the header
	header *free_chunk = calloc(size, sizeof(*free_chunk));
	if (free_chunk == NULL)
	{
		fprintf(stderr, "cannot allocate header\n");
		return -1;
	}
	free_chunk->status = FREE;
	free_chunk->size = size;

	// update the heap and alloc the freelist
	Heap.heapMem = free_chunk;
	Heap.heapSize = size;
	Heap.freeList = malloc(sizeof(Heap.freeList) * size / MIN_CHUNK);
	if (Heap.freeList == NULL)
	{
		fprintf(stderr, "cannot allocate Heap.freelist\n");
		return -1;
	}
	free_chunk++;
	Heap.freeList[0] = free_chunk;
	Heap.freeElems = size / MIN_CHUNK;
	Heap.nFree = 1;
	return 0;
}

/** Release resources associated with the heap. */
void freeHeap(void)
{
	free(Heap.heapMem);
	free(Heap.freeList);
}

/** Allocate a chunk of memory large enough to store `size' bytes. */
void *myMalloc(int size)
{
	// round up the size
	if (size < 1)
		return NULL;
	if (size % 4 != 0)
	{
		int tmp = (4 - size % 4) + size;
		size = tmp;
	}

	// find the smallest free chunk
	header *sfc = (header *)find_sfc(size);
	if (sfc == NULL)
	{
		fprintf(stderr, "cannot find a suitable free chunk\n");
		exit(EXIT_FAILURE);
	}
	// If the free chunk is smaller than N + HeaderSize + MIN_CHUNK,
	// allocate the whole chunk. If the free chunk is larger than this,
	// then split it into two chunks, with the lower chunk allocated
	// for the request, and the upper chunk being a new free chunk.
	if (sfc->size < size + sizeof(header) + MIN_CHUNK)
	{
		sfc->status = ALLOC;
		sfc->size = size + sizeof(header);
		sfc++;
		list_remove(sfc);
	}
	else
	{
		uint tot_size = sfc->size;
		sfc->status = ALLOC;
		sfc->size = size + sizeof(header);
		addr tmp = (addr)sfc + sfc->size;
		header *newfree = (header *)tmp;
		newfree->status = FREE;
		newfree->size = tot_size - sfc->size;
		sfc++;
		list_remove(sfc);
		newfree++;
		list_insert(newfree);
	}
	return sfc;
}

/** Deallocate a chunk of memory. */
void myFree(void *obj)
{
	header *chunk = (header *)obj;
	chunk--;

	// each time after list remove, merge any adjacent free chunks
	addr curr = (addr)Heap.heapMem;
	while (curr < heapMaxAddr())
	{
		header *h = (header *)curr;
		if ((addr)chunk == (addr)h)
		{
			// the only valid case, otherwise print error message
			if (chunk->status == ALLOC)
			{
				chunk->status = FREE;
				chunk++;
				list_insert(chunk);
				merge();
				return;
			}
			else
			{
				break;
			}
		}
		curr += h->size;
	}
	// if the argument to myFree() does not represent an allocated chunk
	// in the heap or is an address somewhere in the middle of an
	// allocated block, print error message
	fprintf(stderr, "Attempt to free unallocated chunk\n");
	exit(1);
}

/** Return the first address beyond the range of the heap. */
static addr heapMaxAddr(void)
{
	return (addr)Heap.heapMem + Heap.heapSize;
}

/** Convert a pointer to an offset in the heap. */
int heapOffset(void *obj)
{
	addr objAddr = (addr)obj;
	addr heapMin = (addr)Heap.heapMem;
	addr heapMax = heapMaxAddr();
	if (obj == NULL || !(heapMin <= objAddr && objAddr < heapMax))
		return -1;
	else
		return (int)(objAddr - heapMin);
}

/** Dump the contents of the heap (for testing/debugging). */
void dumpHeap(void)
{
	int onRow = 0;

	// We iterate over the heap, chunk by chunk; we assume that the
	// first chunk is at the first location in the heap, and move along
	// by the size the chunk claims to be.
	addr curr = (addr)Heap.heapMem;
	while (curr < heapMaxAddr())
	{
		header *chunk = (header *)curr;

		char stat;
		switch (chunk->status)
		{
		case FREE:
			stat = 'F';
			break;
		case ALLOC:
			stat = 'A';
			break;
		default:
			fprintf(
				stderr,
				"myHeap: corrupted heap: chunk status %08x\n",
				chunk->status);
			exit(1);
		}

		printf(
			"+%05d (%c,%5d)%c",
			heapOffset((void *)curr),
			stat, chunk->size,
			(++onRow % 5 == 0) ? '\n' : ' ');
		curr += chunk->size;
	}

	if (onRow % 5 > 0)
		printf("\n");
}
