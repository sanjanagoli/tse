/*
* Sanjana Goli, April 2019
* Lab 5 (Tiny Search Engine) - indextest.c
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
	if(argc == 3) {

		char *oldIndexFilename = (char *)malloc((strlen(argv[1])+1)*sizeof(char));
		strcpy(oldIndexFilename, argv[1]);
		char *newIndexFilename = (char *)malloc((strlen(argv[2])+1)*sizeof(char));
		strcpy(newIndexFilename, argv[2]);

		int sizeOfTable = (int) (get_num_lines(oldIndexFilename)/10);
		
		index_t *index2 = index_new(sizeOfTable);

		//******need to check if the file exists
		index_load(oldIndexFilename, index2);
		
		index_save(newIndexFilename, index2);

		index_delete(index2);

		free(oldIndexFilename);
		free(newIndexFilename);
		printf("%d", sizeOfTable);
		printf("reached indextest");

	} else {
		fprintf(stderr, "usage: ./indextest oldIndexFilename newIndexFilename");
	}
}