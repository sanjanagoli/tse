/*
* Sanjana Goli, April 2019
* Lab 4 (Tiny Search Engine) - pagedir.h
*
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "webpage.h"
#include <string.h>
#include <math.h>

/* check_directory: takes in string filename, returns
					whether directory exists */
bool check_directory(char *filename);


/*  savepage method designed to write to file in specified directory;
  	assumption: none of the filenames in directory are integers  */
void savepage(webpage_t *page, int id, char *pathname);

/*  isCrawlerDirectory method checks whether the directory given was produced
 *	by crawler */
bool isCrawlerDirectory(char *directory);

void openFile(int id, char *directoryName);
