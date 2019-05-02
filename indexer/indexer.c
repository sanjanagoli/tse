/*
* Sanjana Goli, April 2019
* Lab 5 (Tiny Search Engine) - indexer.c
*
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "index.h"
#include "pagedir.h"
#include "webpage.h"
#include "counters.h"


int
main(int argc, char *argv[])
{

	//create variables for inputs to manipulate later
	if(argc == 3) {
		char *directory = (char *)malloc((strlen(argv[1]))*sizeof(char));
		strcpy(directory, argv[1]);
		char *indexFilename = (char *)malloc((strlen(argv[2]))*sizeof(char));
		strcpy(indexFilename, argv[2]);

		//checks whether the directory is produced by crawler
		if (isCrawlerDirectory(directory)) {
			index_t *index = index_new();
			index_build(directory, index);
			index_save(indexFilename, index);
			printf("ayyaya\n");
		}


	} else {
		fprintf(stderr, "usage: ./indexer pageDirectory indexFilename");
		return 1;
	}
	

	
	
}