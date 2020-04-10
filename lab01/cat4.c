// COMP1521 19T2 ... lab 1
// cat4: Copy input to output

#include <stdio.h>
#include <stdlib.h>

static void copy (FILE *, FILE *);

int main (int argc, char *argv[])
{
	if (argc == 1) {
    	copy (stdin, stdout);
	} else {  
	    for(int i = 1; i < argc; i ++) {
	        FILE *fp;
	        fp = fopen(argv[i], "r");
	        if (fp == NULL) {
	            perror("Can't read name-of-file\n");
	        } else {
	            copy (fp, stdout);
	        }
	        fclose(fp); 
	    }    
	}
	return EXIT_SUCCESS;
}

// Copy contents of input to output, char-by-char
// Assumes both files open in appropriate mode
static void copy (FILE *input, FILE *output)
{
    char str[BUFSIZ];
    while(fgets (str, BUFSIZ, input) != NULL) {
        fputs(str, output);
    }
}
