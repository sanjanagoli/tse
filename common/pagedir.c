/*
* Sanjana Goli, April 2019
* Lab 4 (Tiny Search Engine) - pagedir.c
*
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "pagedir.h"
#include <math.h>
#include "webpage.h"

bool check_directory(char *filename);
void savepage(webpage_t *page, int id, char *pathname);

/* check_directory: takes in string filename, returns
					whether directory exists */
bool
check_directory(char *filename)
{	
	FILE *fp;
	if ( (fp = fopen(filename, "w")) != NULL) {	
		fclose(fp);
		return true;
	} else {
		fprintf(stderr, "Directory does not exist!\n");
		return false;
	}
}


/*  savepage method designed to write to file in specified directory;
  	assumption: none of the filenames in directory are integers  */
void
savepage(webpage_t *page, int id, char *pathname)
{
	//digits taken from https://stackoverflow.com/questions/3068397/finding-the-length-of-an-integer-in-c
	int digits = floor(log10(abs(id))) + 1;
	char strId[digits];

	sprintf(strId, "%d", id);

	char *filename = (char *)malloc((strlen(pathname)+digits+2)*sizeof(char));

	//creates filename in formation of directory/id
	strcpy(filename, pathname);
	strcat(filename, "/");
	strcat(filename, strId);


	//printing to file
	FILE * fp;
	if( (fp = fopen(filename, "w")) != NULL ) {
		//add url to beginning of file, followed by depth and HTML
		fprintf(fp, "%s\n", webpage_getURL(page));
		fprintf(fp, "%d\n", webpage_getDepth(page));
		fprintf(fp, "%s\n", webpage_getHTML(page));

		fclose(fp);
	} else {
		//exit if program cannot write to file
		fprintf(stderr, "Error: program could not write to new file in directory");
		exit(1);
	}
	free(filename);		
}
