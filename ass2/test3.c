// COMP1521 19t2 ... Assignment 2: heap management system
// test3.c: read malloc/free ops and do them

#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "myHeap.h"

static void runCommand (void **vars, const char *line);
static void runMalloc (void **vars, char var, int size);
static void runFree (void **vars, char var);
static bool validVar (char c);
static void dumpVars (void **vars);

int main (int argc, char *argv[])
{
	if (argc < 2)
		errx (EX_USAGE, "usage: %s <size>", argv[0]);

	int heapSize = atoi (argv[1]);
	if (initHeap (heapSize) < 0)
		errx (EX_SOFTWARE, "can't init heap of size %d", heapSize);

	dumpHeap ();

	// array of 26 pointer variables
	void *vars[26];
	for (int i = 0; i < 26; i++)
		vars[i] = NULL;

	// read malloc/free commands and carry them out
	char line[BUFSIZ];
	while (fgets (line, BUFSIZ, stdin) != NULL) {
		line[strcspn (line, "\n")] = '\0';

		runCommand (vars, line);

		dumpVars (vars);
		dumpHeap ();
	}

	dumpHeap ();

	return EXIT_SUCCESS;
}

static void runCommand (void **vars, const char *line)
{
	char var;
	int size;

	if (sscanf (line, "%c = malloc %d", &var, &size) == 2) {
		if (! validVar (var))
			warnx ("invalid variable `%c'", var);
		else
			runMalloc (vars, var, size);


	} else if (sscanf (line, "free %c", &var) == 1) {
		if (! validVar (var))
			warnx ("invalid variable `%c'", var);
		else
			runFree (vars, var);

	} else {
		warnx ("ignoring unknown command: %s", line);
	}
}

static void runMalloc (void **vars, char var, int size)
{
	if ((vars[var - 'a'] = myMalloc (size)) == NULL)
		errx (EX_SOFTWARE, "couldn't allocate %d bytes for `%c'", size, var);
}

static void runFree (void **vars, char var)
{
	myFree (vars[var - 'a']);
	vars[var - 'a'] = NULL;
}

static bool validVar (char c)
{
	return 'a' <= c && c <= 'z';
}

// prints allocated variables
// may be helpful for debugging
static void dumpVars (void **vars)
{
	int onRow = 0;
	for (int i = 0; i < 26; i++) {
		if (vars[i] == NULL)
			continue;
		printf ("[%c] +%05d ", 'a' + i, heapOffset (vars[i]));
		onRow++;
		if (onRow == 5) {
			printf ("\n");
			onRow = 0;
		}
	}
	if (onRow != 0)
		printf ("\n");
}
